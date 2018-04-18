#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "libjson.h"
#include "string_buffer.h"

static __string_buffer_t *
json_array_to_string(const struct json_array *this);
static __string_buffer_t *
json_object_to_string(const struct json_object *this);


static __string_buffer_t *
json_object_to_string(const struct json_object *this) {
  struct json_common *p;
  __string_buffer_t *sb, *sb_next;


  if(this == NULL || this->member == NULL) {
    return NULL;
  }

  sb = string_buffer_new();

  sb_next = string_buffer_append(sb, "{");
  //putchar('{');

  for(p = this->member; p != NULL; p = p->next) {
    sb_next = string_buffer_append(sb_next, "\"");
    sb_next = string_buffer_append(sb_next, p->key);
    sb_next = string_buffer_append(sb_next, "\":");

    switch(p->type) {
      case NUMBER:
        sb_next = string_buffer_append_long(sb_next, _JSON_PTR(number, p)->value);
        //printf("\"%s\":%ld", p->key, _JSON_PTR(number, p)->value);
        break;

      case DOUBLE:
        sb_next = string_buffer_append_double(sb_next, _JSON_PTR(double, p)->value);
        //printf("\"%s\":%f", p->key, _JSON_PTR(double, p)->value);
        break;

      case BOOLEAN:
        sb_next = string_buffer_append(sb_next, (_JSON_PTR(boolean, p)->value == TRUE ? "true" : "false"));
        //printf("\"%s\":%s", p->key, (_JSON_PTR(boolean, p)->value == TRUE ? "true" : "false"));
        break;

      case STRING:
        sb_next = string_buffer_append(sb_next, "\"");
        sb_next = string_buffer_append(sb_next, _JSON_PTR(string, p)->value);
        sb_next = string_buffer_append(sb_next, "\"");
        //printf("\"%s\":\"%s\"", p->key, _JSON_PTR(string, p)->value);
        break;

      case ARRAY:
        sb_next->next = json_array_to_string((const struct json_array *)p);
        //printf("\"%s\":", p->key);
        //json_array_to_string((const struct json_array *)p);
        break;

      case OBJECT:
        sb_next->next = json_object_to_string((const struct json_object *)p);
        //printf("\"%s\":", p->key);
        break;

      case NIL:
        sb_next = string_buffer_append(sb_next, "null");
        //printf("\"%s\":null", p->key);
        break;
    }

    if(p->next != NULL) {
      string_buffer_append(sb_next, ",");
      //putchar(',');
    }
  }

  string_buffer_append(sb_next, "}");
  //putchar('}');

  return sb;
}

static __string_buffer_t *
json_array_to_string(const struct json_array *this) {
  struct json_common *p;
  __string_buffer_t *sb, *sb_next;

  sb = string_buffer_new();
  sb_next = string_buffer_append(sb, "[");

  //putchar('[');

  for(p = this->items; p != NULL; p = p->next) {
    switch(p->type) {
      case NUMBER:
        sb_next = string_buffer_append_long(sb_next, _JSON_PTR(number, p)->value);
        //printf("%ld", _JSON_PTR(number, p)->value);
        break;

      case DOUBLE:
        sb_next = string_buffer_append_double(sb_next, _JSON_PTR(double, p)->value);
        //printf("%f", _JSON_PTR(double, p)->value);
        break;

      case BOOLEAN:
        sb_next = string_buffer_append(sb_next,
            (_JSON_PTR(boolean, p)->value == TRUE ? "true" : "false"));
        //printf("%s", (_JSON_PTR(boolean, p)->value == TRUE ? "true" : "false"));
        break;

      case STRING:
        sb_next = string_buffer_append(sb_next, "\"");
        sb_next = string_buffer_append(sb_next, _JSON_PTR(string, p)->value);
        sb_next = string_buffer_append(sb_next, "\"");
        //printf("\"%s\"", _JSON_PTR(string, p)->value);
        break;

      case ARRAY:
        sb_next->next = json_array_to_string((const struct json_array *)p);
        //json_array_to_string((const struct json_array *)p);
        break;

      case OBJECT:
        sb_next->next = json_object_to_string((const struct json_object *)p);
        //json_object_to_string((const struct json_object *)p);
        break;

      case NIL:
        sb_next = string_buffer_append(sb_next, "null");
        //printf("null", p->key);
        break;
    }

    if(p->next != NULL) {
      sb_next = string_buffer_append(sb_next, ",");
      //putchar(',');
    }
  }

  sb_next = string_buffer_append(sb_next, "]");
  //putchar(']');

  return sb;
}

char *
json_to_string(const struct json_common *this) {
  char *buf;
  __string_buffer_t *sb;

  if(this == NULL || (this->type != OBJECT && this->type != ARRAY)) {
    return NULL;
  }

  if(this->type == OBJECT) {
    sb = json_object_to_string((const struct json_object *)this);
  } else if(this->type == ARRAY) {
    json_array_to_string((const struct json_array *)this);
  }

  buf = string_buffer_to_string(sb);
  string_buffer_free(sb);
  sb = NULL;

  return buf;
}
