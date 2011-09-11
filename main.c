/*
clang -o dm3tool -Wall main.c
*/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>

#include "dm3.h"

static struct option options[] = {
  { "help", no_argument, NULL, 'h' },
  { }
};

static void usage() {
  fprintf(stderr,
"usage: dm3tool [options] inputfile.dm3\n"
"\n"
"options:\n"
"  -o FILE  write image data to FILE\n"
          );
}

static void fatal(const char* msg, ...) {
  va_list args;
  va_start(args, msg);
  vfprintf(stderr, msg, args);
  va_end(args);
  exit(1);
}

int main(int argc, char* argv[]) {
  FILE* in_file;

  // Parse options.
  const char* input_file = NULL;
  const char* output_file = NULL;

  int opt;
  while ((opt = getopt_long(argc, argv, "o:h", options, NULL)) != -1) {
    switch (opt) {
      case 'o':
        output_file = optarg;
        break;
      case 'h':
      default:
        usage();
        return 1;
    }
  }
  argv += optind;
  argc -= optind;

  if (argc != 1) {
    usage();
    return 1;
  }
  input_file = argv[0];

  // Read input.
  in_file = fopen(input_file, "rb");
  if (!in_file)
    fatal("Unable to read \'%s\'\n", in_file);

  fclose(in_file);

  return 0;
}
