/*===========================================
  Authors:     Ghiles Maloum - Lucas Benesby
  Created:     11/11/2024
  Modified:    --/--/----
  =========================================== */

#ifndef _PARSE_ARG_H
#define _PARSE_ARG_H

#include "verbose.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/// @brief write message if invalid arg
/// @param arg arg that doesn't work
void error_arg(char arg);

/// @brief parse alpha and beta option
/// @param flag_a if option alpha is specified
/// @param alpha_str alpha specified in argument
/// @param alpha alpha parse with alpha_str
/// @param flag_b if option beta is specified
/// @param beta_str beta specified in argument
/// @param beta beta parse with beta_str
/// @param flag_c if option encoding is specified
/// @param verbose 1 if verbose mode is enabled, 0 otherwise
/// @return 0 if the parsing was successful, -1 otherwise.
int parse_ab(int flag_a, char* alpha_str, double *alpha, 
             int flag_b, char* beta_str, double *beta, int flag_c, int verbose);

/// @brief print help option
void print_help();

/// @brief manage error for encoding/decoding/input
/// @param flag_c if option encoding is specified
/// @param flag_u if option decoding is specified
/// @param flag_i if option input is specified
/// @return 0 if options are correctly specified, -1 otherwise.
int manage_CUI(int flag_c, int flag_u, int flag_i);

#endif