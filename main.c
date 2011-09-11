/*
clang -o dm3tool -Wall main.c
*/

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "dm3.h"

static struct option options[] = {
  { "dump", no_argument, NULL, 'd' },
  { "help", no_argument, NULL, 'h' },
  { }
};

static void usage() {
  fprintf(stderr,
"usage: dm3tool [options] inputfile.dm3\n"
"\n"
"options:\n"
"  -o FILE  write image data to FILE\n"
"  -d       dump file tree to stdout\n"
          );
}

static void fatal(const char* msg, ...) {
  va_list args;
  va_start(args, msg);
  vfprintf(stderr, msg, args);
  va_end(args);
  exit(1);
}

static DM3uint32 dm3_uint32(struct DM3Image* dm3, DM3uint32 i) {
  if (!dm3->is_little_endian || dm3->is_little_endian == 0x01000000)
    return ntohl(i);
  return i;  // Assumes a little-endian machine.
}

static void dump_dm3_tag_group(struct DM3Image* dm3, struct DM3TagGroup* tags) {
}

static void dump_dm3(struct DM3Image* dm3) {
  uint32_t version = dm3_uint32(dm3, dm3->version);
  printf("Version: %u\n", version);
  printf("Length: %u\n", dm3_uint32(dm3, dm3->length));
  printf("Is little-endian: %u\n", dm3_uint32(dm3, dm3->is_little_endian));
  if (version != 3)
    fatal("Unsupported file version\n");
  dump_dm3_tag_group(dm3, &dm3->tag_group);
}

int main(int argc, char* argv[]) {
  int in_file;
  struct stat in_stat;
  struct DM3Image* in_dm3;

  // Parse options.
  _Bool do_dump = 0;
  const char* in_name = NULL;
  const char* out_name = NULL;

  int opt;
  while ((opt = getopt_long(argc, argv, "o:dh", options, NULL)) != -1) {
    switch (opt) {
      case 'd':
        do_dump = 1;
        break;
      case 'o':
        out_name = optarg;
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
  in_name = argv[0];

  // Read input.
  in_file = open(in_name, O_RDONLY);
  if (!in_file)
    fatal("Unable to read \'%s\'\n", in_name);

  if (fstat(in_file, &in_stat))
    fatal("Failed to stat \'%s\'\n", in_name);

  // Casting memory like this is not portable.
  in_dm3 = (struct DM3Image*)mmap(/*addr=*/0, in_stat.st_size,
                                  PROT_READ, MAP_SHARED, in_file, /*offset=*/0);
  if (in_dm3 == MAP_FAILED)
    fatal("Failed to mmap: %d (%s)\n", errno, strerror(errno));

  if (do_dump)
    dump_dm3(in_dm3);

  munmap(in_dm3, in_stat.st_size);
  close(in_file);

  return 0;
}
