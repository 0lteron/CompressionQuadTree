/*===========================================
  Authors:     Ghiles Maloum - Lucas Benesby
  Created:     28/10/2024
  Modified:    10/12/2024
  =========================================== */

#include "pgm_io.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


int readPGM(const char *filename, unsigned char **pixmap, size_t *width,
            size_t *height, unsigned char *grayScale, int verbose) {

  assert(filename != NULL);

  // verbose message
  char message[100];
  sprintf(message, "\x1b[1;32mReading PGM file:\x1b[0m \x1b[1;35m%s\x1b[0m",
          filename);
  print_verbose(verbose, message);

  FILE *file = fopen(filename, "rb");
  if (file == NULL)
    return -1;

  print_verbose(verbose, "\tReading the magic number...");
  char magicNumber[3];
  if (fscanf(file, "%s\n", magicNumber) != 1) {
    fclose(file);
    return -1;
  }
  magicNumber[2] = '\0';

  if (magicNumber[0] != 'P' || magicNumber[1] != '5') {
    fclose(file);
    return -1;
  }

  // Skip comments
  // If the first character is a '#', we skip the line
  for (char c = fgetc(file); c == '#' && c != EOF; c = fgetc(file)) {
    char d;
    while ((d = fgetc(file)) != '\n' && d != EOF)
      ;
  }

  // Go back one character because we read one character too many in the loop
  fseek(file, -1, SEEK_CUR);
  // Read the width and height of the pixmap
  size_t w, h;
  if (fscanf(file, "%zu %zu", &w, &h) != 2 || w <= 0 || h <= 0) {
    fclose(file);
    return -1;
  }

  print_verbose(verbose, "\tReading the grayscale value...");
  // Read the grayscale value
  size_t g;
  if (fscanf(file, "%zu", &g) != 1 || g != 255) {
    fclose(file);
    return -1;
  }

  sprintf(message,
          "\tAllocating memory for the pixmap: \x1b[1;35m%zux%zu\x1b[0m", w, h);
  print_verbose(verbose, message);

  unsigned char *pixmp = (unsigned char *)malloc(w * h);
  if (pixmp == NULL) {
    fclose(file);
    return -1;
  }

  // skip the newline character
  if (fgetc(file) == EOF) {
    fclose(file);
    return -1;
  }

  print_verbose(verbose, "\tReading pixmap width and height...");

  // reads the pixmap data
  if (fread(pixmp, 1, w * h, file) != w * h) {
    free(pixmp);
    fclose(file);
    return -1;
  }
  fclose(file);

  *pixmap = pixmp;
  *width = w;
  *height = h;
  *grayScale = g;

  print_verbose(verbose, "\x1b[1;32mReading successful!\n\x1b[0m");
  return 0;
}

int writePGM(const char *filename, unsigned char *pixmap, size_t width,
             unsigned char grayScale, char *comments, int verbose) {
  assert(filename != NULL);
  assert(pixmap != NULL);
  assert(width > 0);

  char message[100];
  sprintf(message, "\x1b[1;32mWriting PGM file:\x1b[0m \x1b[1;35m%s\x1b[0m",
          filename);
  print_verbose(verbose, message);
  FILE *file = fopen(filename, "wb");
  if (file == NULL) {
    return -1;
  }

  print_verbose(verbose, "\tWriting the magic number...");
  // Write the magic number
  fprintf(file, "P5\n");

  // Write the comments
  if (comments != NULL) {

    fprintf(file, "%s", comments);
  }
  print_verbose(verbose, "\tWriting the width and height...");
  // Write the width and height
  fprintf(file, "%zu %zu\n", width, width);

  print_verbose(verbose, "\tWriting the grayscale value...");
  // Write the grayscale value
  fprintf(file, "%u\n", grayScale);

  print_verbose(verbose, "\tWriting the pixmap data...");
  // Write the pixmap data
  fwrite(pixmap, sizeof(unsigned char), width * width, file);

  print_verbose(verbose, "\x1b[1;32mWriting successful!\x1b[0m");

  fclose(file);

  print_verbose(verbose, "\x1b[1;32mSaving the file...\n\x1b[0m");
  return 0;
}