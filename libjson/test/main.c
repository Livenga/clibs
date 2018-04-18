#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <libjson.h>


int
main(int argc, char *argv[]) {
  char *buf;
  struct json_object *object, *in_object;

  object    = json_object_new(NULL);
  in_object = json_object_new("in_object_1");

  json_add_member(object, (struct json_common *)json_string_new("key1", "value1"));
  json_add_member(object, (struct json_common *)json_string_new("key2", "value2"));
  json_add_member(object, (struct json_common *)json_string_new("key3", "value3"));
  json_add_member(object, (struct json_common *)json_string_new("key4", "value4"));
  json_add_member(object, (struct json_common *)json_string_new("key5", "value5"));

  json_add_member(object, (struct json_common *)in_object);
  json_add_member(in_object, (struct json_common *)json_number_new("number1", 1204));

  // 文字列変換
  buf = json_to_string((struct json_common *)object);
  printf("%s\n", buf);
  free((void *)buf); buf = NULL;

  json_object_free(object);

  return 0;
}
