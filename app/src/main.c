#include "parse_arg.h"
#include "qtc.h"
#include "verbose.h"

#include <getopt.h>

int main(int argc, char *argv[]) {

  // define flag to parse
  int flag_c = 0, flag_u = 0, flag_g = 0, flag_v = 0, flag_i = 0, flag_o = 0,
      flag_a = 0, flag_b = 0;
  char *input = NULL, *output = NULL;
  char *alpha_str = NULL, *beta_str = NULL;
  double alpha = 1.5, beta = 0.8;
  int c;
  extern int opterr;
  opterr = 0;

  while ((c = getopt(argc, argv, "hucgvi:o:a:b:")) != -1) {
    switch (c) {
    case 'h':
      print_help();
      return 0;
    case 'u':
      flag_u = 1;
      break;
    case 'c':
      flag_c = 1;
      break;
    case 'g':
      flag_g = 1;
      break;
    case 'v':
      flag_v = 1;
      break;
    case 'i':
      flag_i = 1;
      input = optarg;
      break;
    case 'o':
      flag_o = 1;
      output = optarg;
      break;
    case 'a':
      flag_a = 1;
      alpha_str = optarg;
      break;
    case 'b':
      flag_b = 1;
      beta_str = optarg;
      break;

    default:
      error_arg(optopt);
      return -1;
    }
  }

  // option verbose
  if (flag_v == 1) {
    print_verbose(flag_v, "Verbose option: \x1b[1;32menabled\x1b[0m\n");
  }

  print_verbose(flag_v, "Analyzing program arguments...");

  // parse alpha/beta option
  if (parse_ab(flag_a, alpha_str, &alpha, flag_b, beta_str, &beta, flag_c,
               flag_v) == -1)
    return -1;

  // manage option C (encode) U (decode) I (input)
  if (manage_CUI(flag_c, flag_u, flag_i) == -1)
    return -1;

  if (flag_c == 1) { // encodeur
    print_verbose(flag_v, "\x1b[1;4;32mEncoding mode\n\x1b[0m");
    //  name output file
    if (encodeImage(input, output, alpha, beta, flag_g, flag_v, flag_o))
      return -1;
  } else { // decodeur
    print_verbose(flag_v, "\x1b[1;4;32mDecoding mode\x1b[0m");
    //  name output file
    if (decodeImage(input, output, flag_g, flag_v, flag_o))
      return -1;
  }

  print_verbose(flag_v, "\x1b[1;32mProgram execution successful!\x1b[0m");
  return 0;
}