/*===========================================
  Authors:     Ghiles Maloum - Lucas Benesby
  Created:     10/12/2024
  Modified:    14/12/2024
  =========================================== */

#include "qtc.h"
#include "coder.h"
#include "decoder.h"
#include "file_naming.h"
#include "pgm_io.h"
#include "quadtree.h"
#include "segmentation.h"
#include "verbose.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

int decodeImage(const char *input, char *output, int flag_g, int verbose,
                int flag_o) {
  QuadTree *qt = NULL;
  char *comments = NULL;
  unsigned char grayScale;

  // decode file in qt
  if (QTC_decoder(input, &qt, &grayScale, &comments, verbose) == -1) {
    fprintf(stderr,
            "\x1b[1;31mError\x1b[0m: file could not be correctly parsed\n");
    return -1;
  }

  unsigned char *pixmap = NULL;
  size_t width = pow(2, qt->numLevels);

  // build pixmap
  if (buildPixMap(qt, &pixmap, qt->numLevels, verbose) == -1) {
    fprintf(stderr, "\x1b[1;31mError\x1b[0m: pixmap could not be built\n");
    freeQuadTree(qt);
    return -1;
  }

  char filename_out[64];
  name_output_file(flag_o, output, filename_out, ".pgm", verbose, FALSE);
  // write output
  writePGM(filename_out, pixmap, width, grayScale, comments, verbose);

  // if segmentation, write segmentation
  if (flag_g == 1) {
    char filename_out_segm[64];
    name_output_file(flag_o, output, filename_out_segm, ".pgm", verbose,
                     flag_g);
    make_contoured_white_squares(qt);
    unsigned char *pixmap_segm = NULL;
    if (buildPixMap(qt, &pixmap_segm, qt->numLevels, verbose) == -1) {
      fprintf(stderr, "\x1b[1;31mError\x1b[0m: pixmap could not be built\n");
      freeQuadTree(qt);
      free(pixmap);
      free(comments);
      return -1;
    }
    writePGM(filename_out_segm, pixmap_segm, width, grayScale, comments,
             verbose);
    free(pixmap_segm);
  }

  freeQuadTree(qt);
  free(pixmap);
  free(comments);
  return 0;
}

// print the comments of the QuadTree into comments
int sprintComments(QuadTree *qt, char *comments) {
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  char buffer[32];
  if (strftime(buffer, sizeof(buffer), "%c", &tm) == 0) {
    fprintf(stderr,
            "\x1b[1;31mError\x1b[0m: date could not be written to the file\n");
    return -1;
  }

  size_t totalSize = calculateSize(qt, 0);
  totalSize += __CHAR_BIT__ - (totalSize % __CHAR_BIT__);
  size_t numPixels = 1 << (2 * qt->numLevels);
  float compression_rate = (float)totalSize / (numPixels * __CHAR_BIT__) * 100;
  sprintf(comments, "# %s\n# compression rate %.2f%%\n", buffer,
          compression_rate);
  return 0;
}

int encodeImage(const char *input, char *output, double alpha, double beta,
                int flag_g, int verbose, int flag_o) {
  unsigned char *pixmap;
  size_t width, height;
  unsigned char grayScale;
  char filename_out[64];
  name_output_file(flag_o, output, filename_out, ".qtc", verbose, FALSE);

  // read pgm input
  if (readPGM(input, &pixmap, &width, &height, &grayScale, verbose) == -1) {
    fprintf(stderr,
            "\x1b[1;31mError\x1b[0m: file could not be correctly parsed\n");
    return -1;
  }

  // create QuadTree
  QuadTree *qt = createQuadTree(width, verbose);
  if (qt == NULL) {
    fprintf(stderr, "\x1b[1;31mError\x1b[0m: QuadTree could not be created\n");
    free(pixmap);
    return -1;
  }

  // fill and filter qt
  fillQuadTree(qt, pixmap, width, verbose);
  filterQuadTree(qt, alpha, beta, verbose);

  // encode qt in filename_out
  QTC_encoder(qt, filename_out, verbose);

  // if segmentation, write segmentation
  if (flag_g == 1) {
    char filename_out_segm[64];
    name_output_file(flag_o, output, filename_out_segm, ".pgm", verbose,
                     flag_g);
    make_contoured_white_squares(qt);
    unsigned char *pixmap_segm = NULL;
    if (buildPixMap(qt, &pixmap_segm, qt->numLevels, verbose) == -1) {
      fprintf(stderr, "\x1b[1;31mError\x1b[0m: pixmap could not be built\n");
      freeQuadTree(qt);
      free(pixmap);
      return -1;
    }
    char comments[256];
    sprintComments(qt, comments);
    writePGM(filename_out_segm, pixmap_segm, width, grayScale, comments,
             verbose);
    free(pixmap_segm);
  }

  // free
  freeQuadTree(qt);
  free(pixmap);
  return 0;
}
