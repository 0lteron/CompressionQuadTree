/*===========================================
  Authors:     Ghiles Maloum - Lucas Benesby
  Created:     30/10/2024
  Modified:    10/12/2024
  =========================================== */

#include "decoder.h"

#include <assert.h>
#include <stdio.h>


/******************************************************************************
 * Rules for decoding the tree:
 * - The tree is checked from the root to the leaves
 * - the average intensity of the fourth child is calculated with this formula:
 *     m4 = (4m + e) - (m1 + m2 + m3)
 * - If the node is uniform and has an error of 0 then then intensities of the
 *    children are the same as the parent and they are all uniform as well and
 *    error is 0
 * - For the last level, we read the average intensity, the uniformity is et to
 *1 and error to 0
 *
 ******************************************************************************/

/// @brief  Reads n bits from the file
/// @param file  The file to read from
/// @param bitField  The bitField holding the current byte being filled
/// @param bitCount  The count of bits read from the bitField so far
/// @param bits  The bits to read
/// @param n  The number of bits to read
static void readBits(FILE *file, unsigned char *bitField, int *bitCount,
                     unsigned char *bits, int n) {
  /**
   * read n bits from the bitField, it uses bitCount to keep track of where we
   * stopped reading the last time from the bitField
   */
  assert(file != NULL);
  assert(bitField != NULL);
  assert(bitCount != NULL);
  assert(bits != NULL);
  assert(n > 0 && n <= __CHAR_BIT__);
  *bits = 0;
  while (n > 0) {
    // if the bitField is empty, we read a new byte
    if (*bitCount == 0) {
      fread(bitField, sizeof(unsigned char), 1, file);
      *bitCount = __CHAR_BIT__;
    }
    // calculate the number of bits to read
    int bitsToRead = n < *bitCount ? n : *bitCount;
    // calculate the position of the bits to read
    int bitPosition = *bitCount - bitsToRead;
    unsigned char extractedBits =
        (*bitField >> bitPosition) & ((1 << bitsToRead) - 1);
    // add the extracted bits to the bits
    *bits = (*bits << bitsToRead) | extractedBits;
    *bitCount -= bitsToRead;
    n -= bitsToRead;
  }
}

void readTree_aux(FILE *file, QuadTree *qt, unsigned char h,
                  unsigned char *bitField, int *bitCount) {
  unsigned char m, e, u;
  for (size_t index = 0; index < totalNodes(h); index++) {
    Node *node = &qt->root[index];
    // if the parent node is uniform and has an error of 0
    if (index != 0 && qt->root[(index - 1) / 4].e == 0 &&
        qt->root[(index - 1) / 4].u == 1) {
      // set the intensity of the node to the intensity of the parent
      // set the uniformity to 1 and the error to 0
      node->m = qt->root[(index - 1) / 4].m;
      node->u = 1;
      node->e = 0;
      continue;
    }

    // if the node is not the fourth child or the root
    if (index % 4 != 0 || index == 0) {
      // write the intesity m (8 bits)
      readBits(file, bitField, bitCount, &m, __CHAR_BIT__);
      node->m = m;
    } else {
      // calculate the intensity of the fourth child
      node->m =
          (4 * qt->root[(index - 1) / 4].m + qt->root[(index - 1) / 4].e) -
          (qt->root[index - 3].m + qt->root[index - 2].m +
           qt->root[index - 1].m);
    }

    // Check if the node is a leaf we skip writing the uniformity and error
    // bits
    if (index >= totalNodes(h - 1)) {
      node->u = 1;
      node->e = 0;
      continue;
    }

    // Write `e` (2 bits)
    readBits(file, bitField, bitCount, &e, 2);
    node->e = e;
    // Write `u` only if `e == 0` (1 bit)
    if (node->e == 0) {
      readBits(file, bitField, bitCount, &u, 1);
      node->u = u;
    }
  }
  return;
}

/// @brief Reads the quadtree from the file
/// @param qt The quadtree to fill
/// @param h The number of levels of the quadtree
/// @return 0 if the quadtree was read successfully, -1 otherwise
static int readTree(QuadTree **qt, unsigned char h, FILE *file, int verbose) {
  assert(h > 0);
  assert(file != NULL);
  size_t width = (size_t)1 << h;
  // create the quadtree
  if ((*qt = createQuadTree(width, verbose)) == NULL) {
    return -1;
  }
  unsigned char bitField = 0;
  int bitCount = 0;
  readTree_aux(file, *qt, h, &bitField, &bitCount);
  return 0;
}

int QTC_decoder(const char *filename, QuadTree **qt,
                         unsigned char *grayScale, char **comments,
                         int verbose) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    return -1;
  }

  // verbose message
  char message[100];
  sprintf(message, "\x1b[1;32mDecoding file \x1b[0m \x1b[1;35m%s\x1b[0m",
          filename);
  print_verbose(verbose, message);

  char magicNumber[3];
  // read the magic number
  print_verbose(verbose, "\tReading the magic number...");
  fscanf(file, "%c%c%c", &magicNumber[0], &magicNumber[1], &magicNumber[2]);
  if (magicNumber[0] != 'Q' || magicNumber[1] != '1' || magicNumber[2] != '\n') {
    return -1;
  }
  magicNumber[2] = '\0';
  // Save the position where comments start
  long comments_start = ftell(file);

  // First pass to find the end position of comments
  int c;
  size_t commentsSize = 0;
  int inComment = 0;
  while ((c = fgetc(file)) != EOF) {
    if (inComment && c == '\n') {
      inComment = 0;
      commentsSize++; // For the newline character
    } else if ((c == '#' && !inComment) || inComment) {
      // Start of a comment line or in a comment line
      inComment = 1;
      commentsSize++;
    } else if (!inComment && c != '#') {
      // Not a comment, exit loop
      // Go back one character
      ungetc(c, file);
      break;
    }
  }
  // allocate memory for the comments only if there are any
  if (commentsSize != 0) {
    // Allocate the comments
    *comments = malloc(commentsSize + 1); // +1 for '\0'
    if (*comments == NULL) {
      fclose(file);
      return -1;
    }

    // Second pass to read the comments block using fread
    // Go back to the start of the comments block
    if (fseek(file, comments_start, SEEK_SET) != 0) {
      free(*comments);
      fclose(file);
      return -1;
    }
    // Read the comments block
    size_t bytesRead = fread(*comments, 1, commentsSize, file);
    if (bytesRead != commentsSize) {
      free(*comments);
      fclose(file);
      return -1;
    }
    (*comments)[commentsSize] = '\0'; // Null-terminate the string
  }
  print_verbose(verbose, "\tReading the height of the quadtree...");

  unsigned char h;
  // read the height of the quadtree
  fread(&h, sizeof(unsigned char), 1, file);
  print_verbose(verbose, "\t\x1b[1;32mReading the quadtree...\x1b[0m");
  if (readTree(qt, h, file, verbose) == -1) {
    return -1;
  }
  (*qt)->numLevels = h;
  *grayScale = 255;
  sprintf(message, "\x1b[1;32mDecoding successful!\n\x1b[0m");
  print_verbose(verbose, message);
  fclose(file);
  return 0;
}

// Recursive helper function to fill the pixmap
static void buildPixMap_aux(QuadTree *qt, unsigned char *pixmap, size_t width,
                            size_t x, size_t y, size_t nodeSize,
                            size_t nodeIndex) {
  Node *node = &qt->root[nodeIndex];
  if (nodeSize == 1 || (node->e == 0 && node->u == 1)) {
    // If the node is uniform or we've reached the smallest size, fill the
    // region
    for (size_t i = y; i < y + nodeSize; i++) {
      for (size_t j = x; j < x + nodeSize; j++) {
        pixmap[i * width + j] = node->m;
      }
    }
  } else {
    size_t shift = nodeSize / 2;
    size_t childIndex =
        nodeIndex * 4 + 1; // Assuming child order: TL, TR, BR, BL

    // Top-left child (Quadrant 0)
    buildPixMap_aux(qt, pixmap, width, x, y, shift, childIndex);

    // Top-right child (Quadrant 1)
    buildPixMap_aux(qt, pixmap, width, x + shift, y, shift, childIndex + 1);

    // Bottom-right child (Quadrant 2)
    buildPixMap_aux(qt, pixmap, width, x + shift, y + shift, shift,
                    childIndex + 2);

    // Bottom-left child (Quadrant 3)
    buildPixMap_aux(qt, pixmap, width, x, y + shift, shift, childIndex + 3);
  }
}

int buildPixMap(QuadTree *qt, unsigned char **pixmap, unsigned char h,
                int verbose) {
  assert(qt != NULL);
  assert(h > 0);
  size_t width = 1 << h;

  print_verbose(verbose, "\x1b[1;32mBuilding the pixmap...\x1b[0m");

  // Allocate memory for pixmap
  *pixmap = (unsigned char *)malloc(width * width * sizeof(unsigned char));
  if (*pixmap == NULL) {
    return -1;
  }
  // Start recursive filling from root node (index 0)
  buildPixMap_aux(qt, *pixmap, width, 0, 0, width, 0);
  print_verbose(verbose, "\x1b[1;32mPixmap built successfully!\n\x1b[0m");
  return 0;
}
