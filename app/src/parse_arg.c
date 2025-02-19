/*===========================================
  Authors:     Ghiles Maloum - Lucas Benesby
  Created:     11/11/2024
  Modified:    --/--/----
  =========================================== */

#include "parse_arg.h"

void error_arg(char arg) {
  // if option is known
  if (arg == 'i' || arg == 'o' || arg == 'a' || arg == 'b') {
    fprintf(stderr,
            "\x1b[1;31mInvalid option:\x1b[0m -%c, missing argument.\n"
            "-h for more information\n",
            arg);
  } else {
    fprintf(stderr,
            "\x1b[1;31mInvalid option:\x1b[0m -%c\n"
            "-h for more information\n",
            arg);
  }
}

int parse_ab(int flag_a, char *alpha_str, double *alpha, int flag_b,
             char *beta_str, double *beta, int flag_c, int verbose) {
  char message[100];
  // if alpha is specified
  if (flag_a == 1) {
    if (flag_c == 0) {
      fprintf(stderr, "\x1b[1;31mInvalid option:\x1b[0m -a, option only "
                      "available for encoding.\n"
                      "-h for more information\n");
      return -1;
    }
    *alpha = strtod(alpha_str, NULL); // str -> double
    // verbose message
    sprintf(message, "\x1b[4mAlpha value\x1b[0m : \x1b[1;35m%.2f\x1b[0m",
            *alpha);
    print_verbose(verbose, message);
  }

  // if beta is specified
  if (flag_b == 1) {
    if (flag_c == 0) {
      fprintf(stderr, "\x1b[1;31mInvalid option:\x1b[0m -b, option only "
                      "available for encoding.\n"
                      "-h for more information\n");
      return -1;
    }
    *beta = strtod(beta_str, NULL); // str -> double
    // verbose message
    sprintf(message, "\x1b[4mBeta  value\x1b[0m : \x1b[1;35m%.2f\x1b[0m",
            *beta);
    print_verbose(verbose, message);
  }
  return 0;
}

void print_help() {
  printf(
      "Usage: ./codec [options]\n"
      "Options:\n"
      "    -u          : Decoding mode.\n"
      "    -c          : Coding mode.\n"
      "    -i <input>  : Input file (pgm/qtc format) [mandatory].\n"
      "    -o <output> : Output file (pgm/qtc format). Defaults: 'out.pgm'.\n"
      "    -g          : Enable segmentation grid.\n"
      "    -a <number> : Set the alpha value. Default: 1.5. Recommended: 1 < "
      "alpha for optimal rendering.\n"
      "    -b <number> : Set the beta value. Default: 0.8. Recommended: 0 < "
      "beta < 1 for optimal rendering.\n"
      "    -v          : Enable verbose mode. Default: silent.\n"
      "    -h          : Show this help message.\n"
      "\n"
      "Note: The options -a and -b are only allowed in encoding mode.\n");
}

int manage_CUI(int flag_c, int flag_u, int flag_i) {
  // if no action is specified
  if (flag_c == 0 && flag_u == 0) {
    fprintf(stderr, "\x1b[1;31mMissing option:\x1b[0m -c or -u.\n"
                    "-h for more information\n");
    return -1;
  }

  // if no input is specified
  if (flag_i == 0) {
    fprintf(stderr, "\x1b[1;31mMissing option:\x1b[0m -i.\n"
                    "-h for more information\n");
    return -1;
  }
  return 0;
}