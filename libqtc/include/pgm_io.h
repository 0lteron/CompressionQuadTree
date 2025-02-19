/*===========================================
  Authors:     Ghiles Maloum - Lucas Benesby
  Created:     28/10/2024
  Modified:    14/11/2024
  =========================================== */

#ifndef _PGM_IO_H
#define _PGM_IO_H

#include "verbose.h"

#include <stdlib.h>
/// @brief Reads a PGM file and stores the image data in the given image
/// pointer.
/// @param filename name of the PGM file to parse.
/// @param image  pointer to the image data.
/// @param width  pointer to the width of the image.
/// @param height pointer to the height of the image.
/// @param grayScale  Maximum grayscale value.
/// @param verbose 1 if verbose mode is enabled, 0 otherwise.
/// @return 0 if the parsing was successful, -1 otherwise.
int readPGM(const char *filename, unsigned char **image, size_t *width,
            size_t *height, unsigned char *grayScale, int verbose);

/// @brief Writes a PGM file with the given pixmap.
/// @param filename name of the file to write.
/// @param pixmap buffer containing the image data.
/// @param width width of the image.
/// @param grayScale Maximum grayscale value.
/// @param comments Comments to write in the file.
/// @param verbose 1 if verbose mode is enabled, 0 otherwise.
/// @return 0 if the writing was successful, -1 otherwise.
int writePGM(const char *filename, unsigned char *pixmap, size_t width,
             unsigned char grayScale, char *comments, int verbose);

#endif