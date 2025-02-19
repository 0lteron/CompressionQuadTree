/*===========================================
  Authors:     Ghiles Maloum - Lucas Benesby
  Created:     30/10/2024
  Modified:    20/12/2024
  =========================================== */


#ifndef _FILE_NAMING_H
#define _FILE_NAMING_H

/// @brief change name of output files
/// @param flag_o if option output is specified
/// @param output output specified in argument
/// @param filename_out filename for encoding or decoding
/// @param extension extension to put
/// @param verbose 1 if verbose mode is enabled, 0 otherwise
/// @param flag_g 1 if segmentation is enabled, 0 otherwise
void name_output_file(int flag_o, char* output, char* filename_out, char* extension, int verbose, int flag_g);

#endif