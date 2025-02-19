/*===========================================
  Authors:     Ghiles Maloum - Lucas Benesby
  Created:     09/11/2024
  Modified:    10/12/2024
  =========================================== */

#include "segmentation.h"
#include "decoder.h"

#include <assert.h>
#include <stdio.h>
#include <time.h>


/// @brief Create a white square at index level
/// @param qt The quadtree where we will create white squares
/// @param index The node number where the white square will begin
static void whiteSquare(QuadTree *qt, int index) {
  Node *node = &qt->root[index];
  // put white color for the squared*
  node->m = 255;
  node->e = 0;
  node->u = 0;

  // Stop if we are at the last level
  if (index >= totalNodes(qt->numLevels - 1)) {
    return;
  }

  // call the function for all sons
  whiteSquare(qt, 4 * index + 1);
  whiteSquare(qt, 4 * index + 2);
  whiteSquare(qt, 4 * index + 3);
  whiteSquare(qt, 4 * index + 4);
}

/// @brief make the upper black outline of the square
/// @param qt The quadtree
/// @param index The node number where the outline will begin
static void upper_square_contour(QuadTree *qt, int index) {
  Node *node = &qt->root[index];

  // if leaf, set in black
  if (index >= totalNodes(qt->numLevels - 1)) {
    node->m = 0;
    node->e = 0;
    node->u = 0;
    return;
  }

  // call the function for the sons that on the upper side
  upper_square_contour(qt, 4 * index + 1);
  upper_square_contour(qt, 4 * index + 2);
}

/// @brief make the left black outline of the square
/// @param qt The quadtree
/// @param index The node number where the outline will begin
static void left_square_contour(QuadTree *qt, int index) {
  Node *node = &qt->root[index];

  // if leaf, set in black
  if (index >= totalNodes(qt->numLevels - 1)) {
    node->m = 0;
    node->e = 0;
    node->u = 0;
    return;
  }

  // call the function for the sons that on the left side
  left_square_contour(qt, 4 * index + 1);
  left_square_contour(qt, 4 * index + 4);
}

/// @brief Create the outline up left of the square
/// @param qt The quadtree where we will create the outline
/// @param index The node number where the outline will begin
static void contour_square(QuadTree *qt, int index) {
  // call the function for the upper and left side
  // note: all that matters is that you contour a corner
  // so right up , down right, down left are also correct
  upper_square_contour(qt, index);
  left_square_contour(qt, index);
}

/// @brief Make the square white with outline if node is uniform
/// @param qt The quadtree where we will create the square
void make_contoured_white_squares(QuadTree *qt) {
  assert(qt != NULL);

  for (size_t index = 0; index < totalNodes(qt->numLevels); index++) {
    Node *node = &qt->root[index];
    // if uniform node, create white square with outline
    if (node->e == 0 && node->u == 1) {
      whiteSquare(qt, index);
      contour_square(qt, index);
    }
  }
}