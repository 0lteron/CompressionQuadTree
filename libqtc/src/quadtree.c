/*===========================================
  Authors:     Ghiles Maloum - Lucas Benesby
  Created:     28/10/2024
  Modified:    15/11/2024
  =========================================== */

#include "quadtree.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


static unsigned char log_2(size_t n) {
  unsigned char i = 0;
  while (n >>= 1)
    i++;
  return i;
}

/// @brief Calculates the sum of the means of the children of a node.
/// @param root The root of the QuadTree.
/// @param childIndex The index of the first child of the node.
/// @return The sum of the means of the children of the node.
static unsigned int sumMeans(Node *root, size_t childIndex) {
  return root[childIndex].m + root[childIndex + 1].m + root[childIndex + 2].m +
         root[childIndex + 3].m;
}

/// @brief Calculates the variance of a node.
/// @param root The root of the QuadTree.
/// @param index The index of the node.
/// @return The variance of the node.
static double calculateVariance(Node *root, size_t index) {
  double µ = 0.;
  size_t childIndex = 4 * index + 1;
  for (size_t i = 0; i < 4; i++) {
    µ += root[childIndex + i].v * root[childIndex + i].v +
         (root[index].m - root[childIndex + i].m) *
             (root[index].m - root[childIndex + i].m);
  }
  return sqrt(µ) / 4;
}

/// @brief Checks if a node is uniform.
/// @param node The node to check.
/// @param index The index of the node.
/// @return 1 if the node is uniform, 0 otherwise.
static unsigned char isUniform(Node *node, size_t childIndex) {
  // A node is uniform if all its children have the same mean and are uniform
  return (node[childIndex].m == node[childIndex + 1].m &&
          node[childIndex + 1].m == node[childIndex + 2].m &&
          node[childIndex + 2].m == node[childIndex + 3].m) &&
         (node[childIndex].u && node[childIndex + 1].u &&
          node[childIndex + 2].u && node[childIndex + 3].u);
}

static void fillQuadTree_aux(QuadTree *qt, unsigned char *pixmap, size_t x,
                             size_t y, unsigned char level, size_t index,
                             size_t width) {

  if (level == qt->numLevels) { // last level
    // fill the leaf node with the corrsponding pixel value
    qt->root[index].m = pixmap[y * width + x];
    qt->root[index].u = 1;
    qt->root[index].e = 0;
    qt->root[index].v = 0.;
  } else {
    size_t childIndex = 4 * index + 1;

    // calculate the amount of shifting needed to get from a quadrant to the
    // next which means that we are currently calculating the size of a
    // quandrant at a certain level
    // in the current level the size of a quadrant is shiftxshift
    size_t depth_from_bottom = qt->numLevels - level - 1;
    size_t shift = 1 << depth_from_bottom;

    // recursively fill the children of the current node
    fillQuadTree_aux(qt, pixmap, x, y, level + 1, childIndex,
                     width); // top left
    fillQuadTree_aux(qt, pixmap, x + shift, y, level + 1, childIndex + 1,
                     width); // top right
    fillQuadTree_aux(qt, pixmap, x + shift, y + shift, level + 1,
                     childIndex + 2,
                     width); // bottom right
    fillQuadTree_aux(qt, pixmap, x, y + shift, level + 1, childIndex + 3,
                     width); // bottom left

    unsigned int sum = sumMeans(qt->root, childIndex);

    qt->root[index].m = (unsigned char)(sum / 4);

    qt->root[index].e = sum % 4;
    // if the error bit != 0  ==> the block is not uniform
    // we need to stock the uniformity bit of the block only if the error bit is
    // 0
    if (qt->root[index].e == 0)
      qt->root[index].u = isUniform(qt->root, childIndex);

    // calculate the variance of the node
    qt->root[index].v = calculateVariance(qt->root, index);
  }
}

void fillQuadTree(QuadTree *qt, unsigned char *pixmap, size_t width,
                  int verbose) {
  assert(qt != NULL);
  assert(pixmap != NULL);
  assert(width > 0);

  print_verbose(verbose, "\x1b[1;32mFilling the QuadTree...\x1b[0m");
  fillQuadTree_aux(qt, pixmap, 0, 0, 0, 0, width);
  print_verbose(verbose, "\x1b[1;32mQuadTree filled successfully!\n\x1b[0m");
}

size_t totalNodes(unsigned char h) {
  // it's equivalent to the sum of the term 4^i from i=0 to i=numLevels
  // 4⁽ⁿ⁺¹⁾ - 1 / 4 - 1 = 4⁽ⁿ⁺¹⁾ - 1 / 3
  return (size_t)(((size_t)1 << (2 * h + 2)) - 1) / 3;
}

QuadTree *createQuadTree(size_t width, int verbose) {
  assert(width > 0);
  print_verbose(verbose, "\x1b[1;32mCreating the QuadTree...\x1b[0m");
  QuadTree *qt = (QuadTree *)malloc(sizeof(QuadTree));
  if (qt != NULL) {
    qt->numLevels = log_2(width);
    size_t numNodes = totalNodes(qt->numLevels);
    qt->root = (Node *)malloc(sizeof(Node) * numNodes);
    if (qt->root == NULL) {
      free(qt);
      fprintf(stderr, "\x1b[1;31mError\x1b[0m: memory allocation failed\n");
      return NULL;
    }
    print_verbose(verbose, "\x1b[1;32mQuadTree created successfully!\n\x1b[0m");
  }
  return qt;
}

void freeQuadTree(QuadTree *qt) {
  free(qt->root);
  free(qt);
}

size_t calculateSize(QuadTree *qt, size_t index) {
  size_t size = 0;
  Node *node = &qt->root[index];
  // if the node is not the fourth child or is the root
  // we add 8 bits to the size (the average intensity of the node)
  if (index % 4 != 0 || index == 0) {
    size += __CHAR_BIT__;
  }
  // if the node is a leaf node we skip writing the uniformity and error bits
  if (index >= totalNodes(qt->numLevels - 1))
    return size;

  if (node->e == 0) {
    size += 3;
    if (node->u == 1)
      return size;
  } else
    size += 2;

  size += calculateSize(qt, 4 * index + 1);
  size += calculateSize(qt, 4 * index + 2);
  size += calculateSize(qt, 4 * index + 3);
  size += calculateSize(qt, 4 * index + 4);
  return size;
}