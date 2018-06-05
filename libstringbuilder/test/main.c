#include <stdio.h>
#include <stdlib.h>

#include "libstringbuilder.h"


int
main(int argc, char *argv[]) {
  char *str;
  struct string_builder *sb;

  sb = string_builder_new();

  string_builder_append(sb, "Hello, ");
  string_builder_append(sb, "World!");

  str = string_builder_to_string(sb);

  printf("%ld\n", string_builder_length(sb));
  printf("%s\n", str);

  free((void *)str);
  str = NULL;
  string_builder_free(sb);
  sb = NULL;

  return 0;
}
