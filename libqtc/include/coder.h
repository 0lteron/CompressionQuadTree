/*===========================================
  Authors:     Ghiles Maloum - Lucas Benesby
  Created:     29/10/2024
  Modified:    10/11/2024
  =========================================== */

#ifndef _CODER_H_
#define _CODER_H_

#include "quadtree.h"

#include <stdio.h>

/// @brief Writes the QuadTree to a file in a lossless format.
/// @param qt The QuadTree to write.
/// @param filename The name of the file to write to.
/// @param verbose 1 if verbose mode is enabled, 0 otherwise.
/// @return 0 if successful, -1 if the file could not be opened.
int QTC_encoder(QuadTree *qt, const char *filename, int verbose);

/// @brief Filters the QuadTree using variance and uniformity.
/// @param qt The QuadTree to filter.
/// @param alpha The threshold for variance.
/// @param beta The second threshold for variance.
/// @param verbose 1 if verbose mode is enabled, 0 otherwise.
void filterQuadTree(QuadTree *qt, double alpha, double beta, int verbose);

#endif