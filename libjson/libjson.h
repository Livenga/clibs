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

struct json_boolean {
  __JSON_COMMON();
  bool_t value;
};

struct json_number {
  __JSON_COMMON();
  long value;
};

struct json_double {
  __JSON_COMMON();
  double value;
};

struct json_array {
  __JSON_COMMON();
  struct json_common *items;
};

struct json_object {
  __JSON_COMMON();
  struct json_common *member;
  //struct json_common *(*add_member)(const struct json_common *, struct json_common *);
};



/* object, value 割当
 */
extern struct json_object *
json_object_new(const char *key);

extern struct json_array *
json_array_new(const char *key);

extern struct json_common *
json_value_new(const int type, const size_t size,
    const char *key, const void *value);

extern struct json_number *
json_number_new(const char *key, const long value);

extern struct json_double *
json_double_new(const char *key, const double value);

extern struct json_boolean *
json_boolean_new(const char *key, const bool_t value);

extern struct json_string *
json_string_new(const char *key, const char *value);


/* object, value, array 追加
 */
extern struct json_common *
json_add(struct json_common *this, struct json_common *target);

extern struct json_object *
json_add_member(struct json_object *this, struct json_common *target);

extern struct json_array *
json_array_add(struct json_array *this, struct json_common *target);


/*
 */
extern struct json_common *
json_value_free(struct json_common *p);

extern void
json_array_free(struct json_array *jarr);

extern void
json_object_free(struct json_object *jo);


/*
 */
extern bool_t
json_contains_key(const struct json_object *jo, const char *key);

extern struct json_common *
json_object_get_value(const struct json_object *jo, const char *key);

extern size_t
json_object_count(const struct json_object *jo);

extern size_t
json_array_count(const struct json_array *ja);


extern void
json_to_string(const struct json_common *this);

#endif
