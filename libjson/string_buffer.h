#ifndef _STRING_BUFFER_H
#define _STRING_BUFFER_H

typedef struct string_buffer __string_buffer_t;

struct string_buffer {
  size_t size;
  char *buf;

  __string_buffer_t *next;
};

extern __string_buffer_t *
string_buffer_new(void);

extern __string_buffer_t *
string_buffer_append(__string_buffer_t *this, const char *value);
extern __string_buffer_t *
string_buffer_append_long(__string_buffer_t *this, const long value);
extern __string_buffer_t *
string_buffer_append_double(__string_buffer_t *this, const double value);

extern void
string_buffer_free(__string_buffer_t *this);

extern size_t
string_buffer_length(const __string_buffer_t *this);

extern char *
string_buffer_to_string(const __string_buffer_t *this);

#endif
