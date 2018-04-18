#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <libstringbuilder.h>

#include "libjson.h"


static void
json_array_to_string(const struct json_array *this);
static void
json_object_to_string(const struct json_object *this);

static void
json_object_to_string(const struct json_object *this) {
  struct json_common *p;


  if(this == NULL || this->member == NULL) {
    return;
  }


  putchar('{');
  for(p = this->member; p != NULL; p = p->next) {
    switch(p->type) {
      case NUMBER:
        printf("\"%s\":%ld", p->key, _JSON_PTR(number, p)->value);
        break;

      case DOUBLE:
        printf("\"%s\":%f", p->key, _JSON_PTR(double, p)->value);
        break;

      case BOOLEAN:
        printf("\"%s\":%s", p->key, (_JSON_PTR(boolean, p)->value == TRUE ? "true" : "false"));
        break;

      case STRING:
        printf("\"%s\":\"%s\"", p->key, _JSON_PTR(string, p)->value);
        break;

      case ARRAY:
        printf("\"%s\":", p->key);
        json_array_to_string((const struct json_array *)p);
        break;

      case OBJECT:
        printf("\"%s\":", p->key);
        json_object_to_string((const struct json_object *)p);
        break;

      case NIL:
        printf("\"%s\":null", p->key);
        break;
    }

    if(p->next != NULL) {
      putchar(',');
    }
  }
  putchar('}');
}

static void
json_array_to_string(const struct json_array *this) {
  struct json_common *p;

  putchar('[');

  for(p = this->items; p != NULL; p = p->next) {
    switch(p->type) {
      case NUMBER:
        printf("%ld", _JSON_PTR(number, p)->value);
        break;

      case DOUBLE:
        printf("%f", _JSON_PTR(double, p)->value);
        break;

      case BOOLEAN:
        printf("%s", (_JSON_PTR(boolean, p)->value == TRUE ? "true" : "false"));
        break;

      case STRING:
        printf("\"%s\"", _JSON_PTR(string, p)->value);
        break;

      case ARRAY:
        json_array_to_string((const struct json_array *)p);
        break;

      case OBJECT:
        json_object_to_string((const struct json_object *)p);
        break;

      case NIL:
        printf("null", p->key);
        break;
    }

    if(p->next != NULL) {
      putchar(',');
    }
  }

  putchar(']');
}

void
json_to_string(const struct json_common *this) {
  if(this == NULL || (this->type != OBJECT && this->type != ARRAY)) {
    return;
  }

  if(this->type == OBJECT) {
    json_object_to_string((const struct json_object *)this);
  } else if(this->type == ARRAY) {
    json_array_to_string((const struct json_array *)this);
  }

}
