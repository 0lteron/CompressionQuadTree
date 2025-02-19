/*===========================================
  Authors:     Ghiles Maloum - Lucas Benesby
  Created:     30/10/2024
  Modified:    10/11/2024
  =========================================== */

#ifndef _DECODER_H_
#define _DECODER_H_

#include "quadtree.h"
#include "verbose.h"

/// @brief Decodes a QuadTree from a file
/// @param filename The name of the file to read from
/// @param qt The QuadTree to fill
/// @param grayScale The grayscale of the image
/// @param comments The comments of the image
/// @param verbose 1 if verbose mode is enabled, 0 otherwise
/// @return 0 if the QuadTree was read successfully, -1 otherwise
int QTC_decoder(const char *filename, QuadTree **qt,
                         unsigned char *grayScale, char **comments, int verbose);

/// @brief Translates the QuadTree into a pixmap
/// @param qt The QuadTree to translate
/// @param pixmap The pixmap to fill
/// @param h The height of the QuadTree
/// @param verbose 1 if verbose mode is enabled, 0 otherwise
/// @return 0 if the pixmap was built successfully, -1 otherwise
int buildPixMap(QuadTree *qt, unsigned char **pixmap, unsigned char h, int verbose);

#endif