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

  //
  //
  //

  FILE *fp;
  char buf[1024];

  string_t *_string;
  string_builder_t *sb, *_sb;

  sb = NULL;
  if((fp = fopen("./random_lines", "r")) != NULL) {
    memset((void *)buf, '\0', sizeof(buf));

    while(fgets((char *)buf, 1024, fp) != NULL) {
      char *_pln;

      if((_pln = strchr(buf, '\n')) != NULL) {
        *_pln = '\0';
      }

      if(sb == NULL) {
        sb  = string_builder_new(buf);
        _sb = sb;
      } else {
        _sb = string_builder_add(_sb, buf);
      }

      memset((void *)buf, '\0', sizeof(buf));
    }

    _string = string_builder_to_string(sb);
    printf("%s\n", _string->string);

    string_free(_string);
    string_builder_free(sb);

    fclose(fp);
  }


  return 0;
}
