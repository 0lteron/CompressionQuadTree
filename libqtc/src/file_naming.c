/*===========================================
  Authors:     Ghiles Maloum - Lucas Benesby
  Created:     20/12/2024
  Modified:    --/--/----
  =========================================== */

#include "file_naming.h"
#include "verbose.h"

#include <stdio.h>
#include <string.h>

static int has_extension(char *filename, char *ext) {
  char *dot = strrchr(filename, '.');
  if (!dot || dot == filename) {
    return 0;
  }
  return strcmp(dot, ext) == 0;
}

void name_output_file(int flag_o, char *output, char *filename_out,
                      char *extension, int verbose, int flag_g) {
  // choose correct directory
  if (strcmp(extension, ".qtc") == 0) {
    strcpy(filename_out, "QTC/");
  } else if (strcmp(extension, ".pgm") == 0) {
    strcpy(filename_out, "PGM/");
  } else {
    fprintf(stderr, "\x1b[1;31mError\x1b[0m: extension not recognized\n");
    return;
  }
    char message[100];

  // if output is specified
  if (flag_o == 1) {
    // verbose message
    char message[100];
    sprintf(message, "\x1b[4mOutput file\x1b[0m: \x1b[1;35m%s\x1b[0m", output);
    print_verbose(verbose, message);

    if (has_extension(output, extension) == 1) {
      // remove the extension
      strrchr(output, '.')[0] = '\0';
    }
    // parse
    strcat(filename_out, output);
  } else {
    strcat(filename_out, "out");
  }
  if (flag_g == 1) {
    strcat(filename_out, "_g");
    sprintf(message,
            "\x1b[4mOutput segmentation file\x1b[0m: \x1b[1;35m%s\x1b[0m",
            filename_out);
  } else {
    sprintf(message, "\x1b[4mOutput file\x1b[0m: \x1b[1;35m%s\x1b[0m",
            filename_out);
  }
  print_verbose(verbose, message);
  // add extension
  strcat(filename_out, extension);
}