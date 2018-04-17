#ifndef _L_JSON_H
#define _L_JSON_H


#define __JSON_COMMON(value_type) \
  int    type; \
  size_t size; \
  struct json_common *next; \
  char   key[__JSON_KEY_SIZE]; \
  struct json_common *(*add)(struct json_common *, struct json_common *)

#define _JSON_PTR(type, p) \
  ((struct json_##type *)p)

#define __JSON_KEY_SIZE 128

#define TRUE 1
#define FALSE 0


typedef unsigned short bool_t;
typedef char * string_t;
typedef void * null_t;


enum json_type {
  STRING = 0,
  NUMBER,
  DOUBLE,
  OBJECT,
  ARRAY,
  BOOLEAN,
  NIL
};


struct json_common {
  __JSON_COMMON();
};

struct json_value {
  __JSON_COMMON();
  void *value;
};

struct json_string {
  __JSON_COMMON();
  string_t value;
};

struct json_number {
  __JSON_COMMON();
  long value;
};

struct json_double {
  __JSON_COMMON();
  double value;
};

struct json_object {
  __JSON_COMMON();
  struct json_common *member;
  //struct json_common *(*add_member)(const struct json_common *, struct json_common *);
};

extern struct json_object *
json_object_new(const char *key);

extern struct json_common *
json_value_new(const int type, const size_t size,
    const char *key, const void *value);

extern void
json_object_free(struct json_object *jo);

extern bool_t
json_contains_key(const struct json_object *jo, const char *key);

extern struct json_common *
json_object_get_value(const struct json_object *jo, const char *key);

extern size_t
json_count(const struct json_object *jo);

extern void
json_print(const struct json_object *jo);

#endif
