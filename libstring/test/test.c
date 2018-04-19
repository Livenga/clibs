#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libstring.h"

static void
_print_string(const string_t *this) {
  const string_t *p_cursor;

  for(p_cursor = this; p_cursor != NULL; p_cursor = p_cursor->next) {
    fprintf(stdout, "%s\n", p_cursor->string);
  }
}


int
main(int argc, char *argv[]) {
  const char *hello = "Hello, World!";

  size_t size;
  char *tmp, **tmp_array;
  string_t *string;

  string = string_split(hello, ",");
  _print_string((const string_t *)string);
  //Hello
  // World!

  tmp_array = string_to_array(string, &size);
  {
    size_t i;
    for(i = 0; i < size; ++i) {
      printf("%3ld: %s\n", i, *(tmp_array + i));
    }
  }
  free((void *)tmp_array);

  string_free(string);


  tmp = string_delete_new("Hello, World! | Test String", "Hello, ");
  fprintf(stdout, "%s\n", tmp);
  // World! | Test String.

  string_delete(tmp, "|");
  fprintf(stdout, "%s\n", tmp);
  // World!  Test String

  free((void *)tmp); tmp = NULL;

#ifdef _ERROR
  // エラーが発生します.
  printf("%s\n", string_delete("Hello, World!", "Hello, "));
#endif

  return 0;
}
