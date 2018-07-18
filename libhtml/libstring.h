#ifndef _LIB_STRING_H
#define _LIB_STRING_H

typedef struct builder_t string_builder_t;

struct builder_t {
  size_t size;
  char *value;

  string_builder_t *next;
};

extern string_builder_t *
string_builder_new(const char *str);

extern string_builder_t *
string_builder_add(string_builder_t *this, const char *str);

extern size_t
string_builder_length(const string_builder_t *this);

extern char *
string_builder_to_string(const string_builder_t *this);

extern void
string_builder_free(string_builder_t *this);

#endif
