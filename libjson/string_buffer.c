#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "string_buffer.h"

#define __string_buffer_alloc() \
  (__string_buffer_t *)calloc(1, sizeof(__string_buffer_t))

static size_t
_numof_digits(long value) {
  int is_minus = 0;
  size_t count;

  is_minus = value < 0;

  value = labs(value);

  count = 0;
  do {
    ++count;
  } while((value /= 10) > 0);

  return count + is_minus;
}

static size_t
_numof_digits_f(double value) {
  return _numof_digits((long)value) + 7;
}


__string_buffer_t *
string_buffer_last_pointer(__string_buffer_t *this) {
  __string_buffer_t *p_cursor, *p;

  if(this == NULL) {
    return NULL;
  }

  for(p_cursor = this; p_cursor != NULL; p_cursor = p_cursor->next) {
    p = p_cursor;
  }

  return p;
}


__string_buffer_t *
string_buffer_new(void) {
  __string_buffer_t *sb;


  sb = __string_buffer_alloc();

  sb->size = 0;
  sb->buf  = NULL;
  sb->next = NULL;

  return sb;
}

__string_buffer_t *
string_buffer_append(__string_buffer_t *this, const char *value) {
  __string_buffer_t *sb, *p_cursor;

  if(this == NULL || value == NULL) {
    return this;
  }

  p_cursor = string_buffer_last_pointer(this);

  sb = __string_buffer_alloc();
  sb->size = sizeof(char) * strlen(value);
  if((sb->buf = (char *)calloc(strlen(value), sizeof(char))) == NULL) {
    return this;
  }

  memmove((void *)sb->buf, (const void *)value, sb->size);
  p_cursor->next = sb;

  return sb;
}


__string_buffer_t *
string_buffer_append_long(__string_buffer_t *this, const long value) {
  __string_buffer_t *sb, *p_cursor;


  if(this == NULL) {
    return NULL;
  }

  p_cursor = string_buffer_last_pointer(this);

  sb = __string_buffer_alloc();
  sb->size = _numof_digits(value);
  if((sb->buf  = (char *)calloc(sb->size, sizeof(char))) == NULL) {
    free((void *)sb);
    return this;
  }
  sprintf(sb->buf, "%ld", value);
  sb->next = NULL;

  p_cursor->next = sb;

  return sb;
}

__string_buffer_t *
string_buffer_append_double(__string_buffer_t *this, const double value) {
  __string_buffer_t *sb, *p_cursor;

  if(this == NULL) {
    return NULL;
  }

  p_cursor = string_buffer_last_pointer(this);

  sb = __string_buffer_alloc();
  sb->size = _numof_digits_f(value);
  if((sb->buf = (char *)calloc(sb->size, sizeof(char))) == NULL) {
    free((void *)sb);
    return this;
  }

  sprintf(sb->buf, "%.6f", value);
  sb->next = NULL;

  p_cursor->next = sb;
  return sb;
}

void
string_buffer_free(__string_buffer_t *this) {
  __string_buffer_t *sb = NULL,
                    *sb_next = NULL;

  if(this == NULL) {
    return;
  }

  sb = this;
  while(sb != NULL) {
    sb_next = sb->next;

    if(sb->buf != NULL) {
      memset((void *)sb->buf, '\0', sizeof(char) * strlen(sb->buf));
      free((void *)sb->buf);
    }
    sb->size = 0L;
    sb->buf  = NULL;
    free((void *)sb);

    sb = sb_next;
  }
}

size_t
string_buffer_length(const __string_buffer_t *this) {
  const __string_buffer_t *p_cursor;
  size_t count;


  if(this == NULL) return -1;

  count = 0L;
  for(p_cursor = this; p_cursor != NULL; p_cursor = p_cursor->next) {
    count += (p_cursor->size > 0) ? p_cursor->size : 0;
  }

  return count;
}

char *
string_buffer_to_string(const __string_buffer_t *this) {
  const __string_buffer_t *p_cursor;
  size_t size;
  char *buf;


  size = string_buffer_length(this);
  if((buf = (char *)calloc(size, sizeof(char))) == NULL) {
    goto end;
  }

  for(p_cursor = this; p_cursor != NULL; p_cursor = p_cursor->next) {
    if(p_cursor->size > 0 && p_cursor->buf != NULL) {
      strncat(buf, (const char *)p_cursor->buf, p_cursor->size);
    }
  }

end:
  return buf;
}
