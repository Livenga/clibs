#include <stdio.h>
#include <stdlib.h>
#include <libstringbuilder.h>


int
main(int argc, char *argv[]) {
  char *str;
  struct string_builder *sb;

  sb = string_builder_new();

  sb->append(sb, "Hello, ")
    ->append(sb, "World!");

  printf("%ld\n", string_builder_length(sb));
  str = string_builder_to_string(sb);

  printf("%s\n", string_builder_to_string(sb));

  free((void *)str);
  string_builder_free(sb);

  return 0;
}
