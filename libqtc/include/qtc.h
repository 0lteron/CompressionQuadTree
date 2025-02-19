/*===========================================
  Authors:     Ghiles Maloum - Lucas Benesby
  Created:     01/12/2024
  Modified:    20/12/2024
  =========================================== */

#ifndef _QTC_H
#define _QTC_H

#include "file_naming.h"

/// @brief encode image input .pgm in output
/// @param input name of file to encode .pgm
/// @param output name of output file
/// @param alpha alpha value
/// @param beta beta value
/// @param segmentation if 0, no segmentation grid; if 1, segmentation is applied.
/// @param verbose 1 if verbose mode is enabled, 0 otherwise.
/// @return 0 if the encoding was successful, -1 otherwise.
int encodeImage(const char *input, char *output, double alpha, double beta, int flag_g, int verbose, int flag_o);

/// @brief decode image .qtc in output
/// @param input name of file to decode .qtc
/// @param output name of output file
/// @param segmentation if 0, no segmentation grid; if 1, segmentation is applied.
/// @param verbose 1 if verbose mode is enabled, 0 otherwise.
/// @return 0 if the decode was successful, -1 otherwise.
int decodeImage(const char *input, char *output, int segmentation, int verbose, int flag_o);

#endif 
