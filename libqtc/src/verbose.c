/*===========================================
  Authors:     Ghiles Maloum - Lucas Benesby
  Created:     30/11/2024
  Modified:    10/12/2024
  =========================================== */

#include "verbose.h"
#include <stdio.h>


void print_verbose(int verbose, const char *message) {
  if (verbose)
    printf("[VERBOSE] %s\n", message);
}
