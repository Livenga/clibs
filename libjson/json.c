#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "libjson.h"

#define _json_alloc(type) \
  (struct json_common *)calloc(1, sizeof(type))


#define _throw(p, point) \
  if(p == NULL) goto point


static int
eprintf(FILE *strm,
    const char *fn,
    const char *param) {
  if(param != NULL)
    return fprintf(strm, "%s: %s %s\n", fn, param, strerror(errno));

  return fprintf(strm, "%s: %s\n", fn, strerror(errno));
}

static struct json_common *
_get_last_value(struct json_common *this) {
  struct json_common *p = this, *p_cursor = NULL;


  if(p == NULL)
    return NULL;

  for(; p != NULL; p = p->next) {
    //printf("%p\n", p);
    p_cursor = p;
  }

  return p_cursor;
}

static char *
_get_type(const int type) {
  switch(type) {
    case STRING:  return "string";
    case NUMBER:  return "number";
    case DOUBLE:  return "number";
    case OBJECT:  return "object";
    case ARRAY:   return "array";
    case BOOLEAN: return "boolean";
    case NIL:     return "null";
  }

  return "Unknown";
}

static struct json_common *
json_value_free(struct json_common *p) {

  // この関数では object, array は開放しない.
  if(p == NULL || p->type == OBJECT || p->type == ARRAY) return p;

  if(p->type == STRING) {
    struct json_string *p_str = NULL;

    p_str = _JSON_PTR(string, p);

    memset((void *)p_str->value, '\0', p->size);
    free((void *)p_str);
    p_str = NULL;
  }

  free((void *)p);
  p = NULL;

  return NULL;
}


struct json_common *
json_add(struct json_common *this, struct json_common *target) {
  struct json_common *p, *p_cursor = this;

  if(this == NULL || target == NULL)
    return NULL;
  
  // object が親要素になる場合以外の key の指定は必須
  if(target->type == OBJECT && strlen(target->key) == 0) {
    fprintf(stderr, "親要素以外の object の key は必須です.");
    return this;
  }

  // key の重複確認
  // NOTE: this に指定された場所以降しか走査を行えないため,
  // その場所以前に key が存在していた場合は, 重複ができてしまう.
  for(; p_cursor != NULL; p_cursor = p_cursor->next) {
    if(strncmp(p_cursor->key, target->key, __JSON_KEY_SIZE) == 0) {
      fprintf(stderr, "指定されたキーは既に登録されています.\n");

      json_value_free(target);
      return this;
    }

    p = p_cursor;
  }

  this = p;

  this->next = target;
  return target;
}

struct json_object *
json_add_member(struct json_object *this, struct json_common *target) {
  if(this == NULL || target == NULL)
    return NULL;

  if(this->member == NULL) {
    this->member = target;
  }

  return this;
}

struct json_object *
json_object_new(const char *key) {
  struct json_object *jo = NULL;

  //
  if(key != NULL && strlen(key) < 1) {
    fprintf(stderr, "オブジェクトを割り当てる場合, "
        "key を null もしくは1文字以上にしてください.");
    return NULL;
  }

  jo         = (struct json_object *)calloc(1, sizeof(struct json_object));
  _throw(jo, error);

  jo->type   = OBJECT;
  jo->size   = -1;
  jo->next   = NULL;
  jo->member = NULL;

  if(key == NULL) {
    memset((void *)jo->key, '\0', sizeof(jo->key));
  } else {
    strncpy(jo->key, key, __JSON_KEY_SIZE);
  }

  return jo;

error:
  eprintf(stderr, "calloc(3)", "* JsonObject の割当に失敗しました.");
  return NULL;
}

struct json_common *
json_value_new(const int type, const size_t size,
    const char *key, const void *value) {
  struct json_common *jvalue = NULL;


  //
  if(key == NULL || strlen(key) == 0) {
    fprintf(stderr, "key を指定してください.");
    return NULL;
  }


  switch(type) {
    case STRING:
      jvalue = _json_alloc(struct json_string);
      _throw(jvalue, error);

      jvalue->type  = type;
      jvalue->size  = size;
      _JSON_PTR(string, jvalue)->value = (char *)calloc(size, sizeof(char));
      memmove(_JSON_PTR(string, jvalue)->value, value, size);
      break;

    case NUMBER:
      jvalue = _json_alloc(struct json_value);
      _throw(jvalue, error);

      jvalue->type = type;
      jvalue->size = sizeof(long);
      memcpy((void *)&_JSON_PTR(number, jvalue)->value, value, sizeof(long));
      //memmove(jvalue->value, value, sizeof(long));
      break;

    case DOUBLE:
      jvalue = _json_alloc(struct json_value);
      _throw(jvalue, error);

      jvalue->type = type;
      jvalue->size = sizeof(double);
      memcpy((void *)&_JSON_PTR(double, jvalue)->value, value, sizeof(double));
      //memmove(jvalue->value, value, sizeof(double));
      break;

    case NIL:
      jvalue = _json_alloc(struct json_value);
      _throw(jvalue, error);

      jvalue->type  = NIL;
      jvalue->size  = 0;
      _JSON_PTR(value, jvalue)->value = NULL;
      break;

    default:
      _throw(NULL, error);
  }

  //
  jvalue->add = json_add;
  jvalue->next = NULL;
  // key
  strncpy(jvalue->key, key, __JSON_KEY_SIZE);


  return jvalue;

error:
  return NULL;
}


void
json_object_free(struct json_object *jo) {
  struct json_common *p = jo->member,
                     *p_next = NULL;
  if(jo == NULL)
    return;

  while(p != NULL) {
    p_next = p->next;

    switch(p->type) {
      case OBJECT:
        json_object_free((struct json_object *)p);
        break;

      case ARRAY:
        break;

      default:
        json_value_free((struct json_common *)p);
    }

    p = p_next;
  }

  free((void *)jo->member);
  jo->member = NULL;

  free((void *)jo);
  jo = NULL;
}


size_t
json_count(const struct json_object *jo) {
  size_t count;
  struct json_common *p = jo->member;

  if(jo == NULL || jo->member == NULL)
    return -1;

  count = 0L;
  for(; p != NULL; p = p->next) {
    ++count;
  }

  return count;
}

bool_t
json_contains_key(const struct json_object *jo, const char *key) {
  struct json_common *p_cursor = jo->member;

  if(jo == NULL || jo->member == NULL) {
    return FALSE;
  }


  for(; p_cursor != NULL; p_cursor = p_cursor->next) {
    if(strncmp(p_cursor->key, key, __JSON_KEY_SIZE) == 0) {
      return TRUE;
    }
  }

  return FALSE;
}

struct json_common *
json_object_get_value(const struct json_object *jo, const char *key) {
  struct json_common *p_cursor = jo->member;

  if(jo == NULL|| jo->member == NULL) {
    return NULL;
  }

  for(; p_cursor != NULL; p_cursor = p_cursor->next) {
    if(strncmp(p_cursor->key, key, __JSON_KEY_SIZE) == 0) {
      return p_cursor;
    }
  }

  return NULL;
}


static size_t num_indent = 1;
void
json_print(const struct json_object *jo) {
  size_t i;
  struct json_common *p;


  if(jo == NULL || jo->member == NULL)
    return;

  for(p = jo->member; p != NULL; p = p->next) {
    for(i = 1; i < num_indent; ++i)
      putchar('\t');

    if(p->type == OBJECT) {
      ++num_indent;
      json_print((const struct json_object *)p);
    }

    switch(p->type) {
      case STRING:
        printf("Type: %s, Key: %s, Value: %s\n",
            _get_type(p->type), p->key, _JSON_PTR(string, p)->value);
        break;

      case NUMBER:
        printf("Type: %s, Key: %s, Value: %ld\n",
            _get_type(p->type), p->key, _JSON_PTR(number, p)->value);
        break;

      case DOUBLE:
        printf("Type: %s, Key: %s, Value: %f\n",
            _get_type(p->type), p->key, _JSON_PTR(double, p)->value);
        break;
    }
  }

  if(num_indent > 1) {
    --num_indent;
  }
}
