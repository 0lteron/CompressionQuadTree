/*===========================================
  Authors:     Ghiles Maloum - Lucas Benesby
  Created:     09/11/2024
  Modified:    20/11/2024
  =========================================== */

#ifndef _SEGMENTATION_H
#define _SEGMENTATION_H

#include "quadtree.h"


/// @brief Transform the QuadTree into a frid of white squares with black
/// contours, this is used for the segmentation of the image
/// @param qt The QuadTree to be changed
void make_contoured_white_squares(QuadTree *qt);

#endif