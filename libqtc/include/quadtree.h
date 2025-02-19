/*===========================================
  Authors:     Ghiles Maloum - Lucas Benesby
  Created:     28/10/2024
  Modified:    10/11/2024
  =========================================== */

#ifndef _QUADTREE_H
#define _QUADTREE_H
#include "verbose.h"

#include <stdlib.h>

typedef struct {
  double v;            // variance
  unsigned char m;     // average intesity of the children
  unsigned char u : 1; // uniformity bit
  unsigned char e : 2; // error bit
} Node;

typedef struct {
  Node *root;
  unsigned char numLevels;
} QuadTree;

/// @brief Calculates the total number of nodes in a quadtree  with h levels.
/// @param h The number of levels
/// @return The total number of nodes
size_t totalNodes(unsigned char h);

/// @brief Creates a QuadTree
/// @param width  The width of the pixmap (since the pixmap is a square no need
/// for the height)
/// @param verbose 1 if verbose mode is enabled, 0 otherwise
/// @return A pointer to the created QuadTree.
QuadTree *createQuadTree(size_t width, int verbose);

/// @brief Initializes the QuadTree with the given pixmap.
/// @param qt The QuadTree to initialize.
/// @param pixmap The pixmap to use.
/// @param width The width of the pixmap (since the pixmap is a square no need
/// for the height).
/// @param verbose 1 if verbose mode is enabled, 0 otherwise
void fillQuadTree(QuadTree *qt, unsigned char *pixmap, size_t width,
                  int verbose);

/// @brief Frees the memory allocated for the QuadTree.
/// @param qt The QuadTree to free.
void freeQuadTree(QuadTree *qt);

/// @brief Calculates the size of the QuadTree in bits.
/// @param qt The QuadTree to calculate the size of.
/// @param index The index of the current node.
/// @return The size of the QuadTree in bits.
size_t calculateSize(QuadTree *qt, size_t index);

#endif