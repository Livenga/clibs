#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "libstring.h"


string_builder_t *
string_builder_new(const char *str) {
  string_builder_t *sb_ptr;


  sb_ptr        = (string_builder_t *)calloc(1, sizeof(string_builder_t));
  if(sb_ptr == NULL) {
    return NULL;
  }

  sb_ptr->size  = strlen(str);
  sb_ptr->value = (char *)calloc(sb_ptr->size + 1, sizeof(char));
  sb_ptr->next  = NULL;

  if(sb_ptr->value == NULL) {
    memset((void *)sb_ptr, '\0', sizeof(string_builder_t));
    free((void *)sb_ptr);
    return (sb_ptr = NULL);
  }

  memcpy((void *)sb_ptr->value, (const void *)str, sizeof(char) * sb_ptr->size);

  return sb_ptr;
}

string_builder_t *
string_builder_add(string_builder_t *this, const char *str) {
  string_builder_t *sb_ptr, *sb_target_ptr, *sb_preptr;


  if(this == NULL && str == NULL) {
    return NULL;
  }

  if(str == NULL) {
    return this;
  }

  if(this == NULL) {
    return string_builder_new(str);
  }

  sb_target_ptr = NULL;
  sb_preptr     = NULL;
  for(sb_target_ptr = this; sb_target_ptr != NULL; sb_target_ptr = sb_target_ptr->next) {
    sb_preptr = sb_target_ptr;
  }
  sb_target_ptr = sb_preptr;


  sb_ptr = string_builder_new(str);
  sb_target_ptr->next = sb_ptr;

  return sb_ptr;
}


size_t
string_builder_length(const string_builder_t *this) {
  const string_builder_t *sb_ptr;
  size_t length;


  if(this == NULL) {
    return -1;
  }

  length = 0;
  for(sb_ptr = this; sb_ptr != NULL; sb_ptr = sb_ptr->next) {
    if(sb_ptr->value != NULL) {
      length += sb_ptr->size;
    }
  }

  return length;
}


char *
string_builder_to_string(const string_builder_t *this) {
  const string_builder_t *sb_ptr;
  char   *buf;
  size_t size, cursor;


  size = string_builder_length(this);
  if(size < 0) {
    return NULL;
  }


  buf    = (char *)calloc(size + 1, sizeof(char));
  if(buf == NULL) {
    return NULL;
  }
  cursor = 0;

  for(sb_ptr = this; sb_ptr != NULL; sb_ptr = sb_ptr->next) {
    if(sb_ptr->value != NULL) {
      memcpy((void *)buf + cursor, (const void *)sb_ptr->value, sizeof(char) * sb_ptr->size);
      cursor += sb_ptr->size;
    }
  }


  return buf;
}


void
string_builder_free(string_builder_t *this) {
  string_builder_t *sb_ptr, *sb_next_ptr;


  if(this == NULL) {
    return;
  }

  do {
    sb_next_ptr = this->next;

    if(this->value != NULL) {
      memset((void *)this->value, '\0', sizeof(char) * this->size);
      free((void *)this->value);
      this->value = NULL;
    }
    memset((void *)this, '\0', sizeof(string_builder_t));
    free((void *)this);

    this = sb_next_ptr;
  } while(this != NULL);
}
