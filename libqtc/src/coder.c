/*===========================================
  Authors:     Ghiles Maloum - Lucas Benesby
  Created:     29/10/2024
  Modified:    10/12/2024
  =========================================== */

#include "coder.h"
#include "quadtree.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <time.h>


/**
 *  Rules for reducing the tree:
 *  - The tree is checked from the root to the leaves
 *  - the average intensity of the fourth child is not considered because it is
 *  implied
 *  - If the node is uniform and has an error of 0, no need to check the
 *  children
 *  - We only to consider the uniformity bit when the error bit is 0
 *  - For the last level, we only need to store the average intensity
 *
 *  **** RULE FOR LOSSY COMPRESSION ****
 *  - If the variance of a node is less than the threshold, we consider the node
 *    as uniform
 */

/// @brief Writes bits to the file, ensuring they are packed into bytes in
/// order from least to most significant bits.
/// @param file The file to write to.
/// @param bitField The bitField holding the current byte being filled.
/// @param bitCount The count of bits written to the bitField so far.
/// @param bits The bits to write.
/// @param numBits The number of bits to write.
static void writeBits(FILE *file, unsigned char *bitField, int *bitCount,
                      unsigned char bits, int numBits) {
  assert(file != NULL);
  assert(bitField != NULL);
  assert(bitCount != NULL);
  assert(numBits <= __CHAR_BIT__);
  assert(numBits >= 0);
  for (int i = 0; i < numBits; i++) {
    // get the i-th from the left
    unsigned char leastSignificantBit = (bits >> (numBits - 1 - i)) & 1U;
    // shift the bitField to the left and add the new bit
    *bitField = (*bitField << 1) | leastSignificantBit;
    (*bitCount)++;
    if (*bitCount == __CHAR_BIT__) { // If bitField is full, write to file
      fwrite(bitField, sizeof(unsigned char), 1, file);
      *bitField = 0; // Reset bitField
      *bitCount = 0; // Reset bit count
    }
  }
}

/// @brief writes the QuadTree structure to a file in binary format.
/// @param file The file to write to.
/// @param qt The QuadTree to write.
/// @param index The index of the current node.
/// @param bitField The current byte being filled.
/// @param bitCount The bit count in the current bitField.
static void writeQuadTree_aux(FILE *file, QuadTree *qt, unsigned char *bitField,
                              int *bitCount) {
  assert(file != NULL);
  assert(qt != NULL);
  assert(bitField != NULL);
  assert(bitCount != NULL);
  for (size_t index = 0; index < totalNodes(qt->numLevels); index++) {
    Node *node = &qt->root[index];
    // if the parent node is uniform and has an error of 0, no need to check the
    // children
    if (index != 0 && qt->root[(index - 1) / 4].e == 0 &&
        qt->root[(index - 1) / 4].u == 1)
      continue;

    // if the node is not the fourth child or is the root
    if (index % 4 != 0 || index == 0)
      // write the intesity m (8 bits)
      writeBits(file, bitField, bitCount, node->m, __CHAR_BIT__);

    // Check if the node is a leaf we skip writing the uniformity and error bits
    if (index >= totalNodes(qt->numLevels - 1))
      continue;

    // Write `e` (2 bits)
    writeBits(file, bitField, bitCount, node->e, 2);
    // Write `u` only if `e == 0` (1 bit)
    if (node->e == 0)
      writeBits(file, bitField, bitCount, node->u, 1);
  }
  return;
}

/// @brief Writes the entire QuadTree to a binary file in specified format.
/// @param qt The QuadTree to write.
/// @param filename The name of the file to write to.
static void writeQuadTree(QuadTree *qt, FILE *file) {
  assert(qt != NULL);
  assert(file != NULL);
  unsigned char bitField = 0;
  int bitCount = 0;
  // Write the QuadTree to the file
  writeQuadTree_aux(file, qt, &bitField, &bitCount);
  // Write any remaining bits
  if (bitCount > 0) {
    bitField <<= (__CHAR_BIT__ - bitCount); // Shift remaining bits to the left
    fwrite(&bitField, sizeof(unsigned char), 1, file);
  }
}

/// @brief Calculates the average and maximum variance of the QuadTree.
/// @param qt The QuadTree to calculate the variance of.
/// @param max maximum variance
/// @param average average variance
static void getAverageMaxVariance(QuadTree *qt, double *max, double *average) {
  assert(qt != NULL);
  assert(max != NULL);
  assert(average != NULL);
  size_t numNodes = totalNodes(qt->numLevels - 1);
  if (numNodes == 0)
    return;
  *max = 0.;
  *average = 0.;
  for (size_t i = 0; i < numNodes; i++) {
    Node *node = &qt->root[i];
    *average += node->v;
    if (node->v > *max)
      *max = node->v;
  }
  *average /= numNodes;
}

int filterQuadTree_aux(QuadTree *qt, size_t index, double sigma, double alpha,
                       double beta) {
  assert(qt != NULL);
  assert(index >= 0);
  assert(sigma >= 0);
  assert(alpha >= 0);

  // if the node is already uniform or a leaf node
  if (qt->root[index].e == 0 && qt->root[index].u == 1)
    return 1;

  size_t childIndex = 4 * index + 1;
  unsigned char uniformize = 1;
  for (size_t i = 0; i < 4; i++)
    // recursively check the children
    // add beta param to increase the quality of compression
    uniformize &= filterQuadTree_aux(qt, childIndex + i, sigma * alpha,
                                     pow(alpha, beta), beta);
  if (!uniformize || qt->root[index].v > sigma)
    return 0;
  // if the children are uniform and the variance is less than the threshold
  // alpha, we consider the node as uniform
  qt->root[index].e = 0;
  qt->root[index].u = 1;
  return 1;
}

void filterQuadTree(QuadTree *qt, double alpha, double beta, int verbose) {
  assert(qt != NULL);
  assert(alpha >= 0);
  assert(beta >= 0);

  print_verbose(verbose, "\x1b[1;32mFiltering the QuadTree...\x1b[0m");
  double maxVar, medVar;
  getAverageMaxVariance(qt, &maxVar, &medVar);
  filterQuadTree_aux(qt, 0, medVar / maxVar, alpha, beta);
  print_verbose(verbose, "\x1b[1;32mFiltering successful!\x1b[0m");
}

int QTC_encoder(QuadTree *qt, const char *filename, int verbose) {
  assert(qt != NULL);
  assert(filename != NULL);

  char message[100];
  sprintf(message,
          "\x1b[1;32mWriting the encoding to\x1b[0m \x1b[1;35m%s\x1b[0m",
          filename);
  print_verbose(verbose, message);

  FILE *file = fopen(filename, "wb");
  if (file == NULL)
    return -1;
  // Write the magic number
  print_verbose(verbose, "\tWriting the magic number");
  fprintf(file, "Q1\n");
  // Write the date and hour of creation of the file
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  char buffer[32];
  if (strftime(buffer, sizeof(buffer), "%c", &tm) == 0) {
    fprintf(stderr, "\x1b[1;31mError:\x1b[0m writing the date to the file\n");
    return -1;
  }
  fprintf(file, "# %s\n", buffer);
  size_t totalSize = calculateSize(qt, 0);
  // round up to the nearest byte
  totalSize += __CHAR_BIT__ - (totalSize % __CHAR_BIT__);
  size_t numPixels = 1 << (2 * qt->numLevels);

  float compression_rate = (float)totalSize / (numPixels * __CHAR_BIT__) * 100;

  sprintf(message, "\tCompression rate: \x1b[1;4;35m%.2f%%\x1b[0m",
          compression_rate);
  print_verbose(verbose, message);

  fprintf(file, "# compression rate %.2f%%\n", compression_rate);
  // write the number of levels
  fwrite(&qt->numLevels, sizeof(unsigned char), 1, file);
  writeQuadTree(qt, file);
  print_verbose(verbose, "\x1b[1;32mEncoding successful!\x1b[0m");
  sprintf(message,
          "\x1b[1;32mSaving the encoding to\x1b[0m \x1b[1;35m%s\x1b[0m\n",
          filename);
  print_verbose(verbose, message);
  fclose(file);
  return 0;
}