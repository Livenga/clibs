#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <libjson.h>


int
main(int argc, char *argv[]) {
  struct json_object *jo;
  long l = 132L;
  double f = 3.14;

  const char *hello = "Hello, World!";


  jo = json_object_new(NULL);

  jo->member = json_value_new(NUMBER, 1, "value", (void *)&l);
  jo->member->add(jo->member, json_value_new(STRING, strlen(hello), "string", (void *)hello));


  printf("%ld\n", json_count(jo));
  json_print(jo);

  json_object_free(jo);
  printf("%ld\n", json_count(jo));

  return 0;
}
