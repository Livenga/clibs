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


  if(p == NULL) {
    return NULL;
  }

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
      //fprintf(stderr, "指定されたキーは既に登録されています.\n");
      json_value_free(target);
      return this;
    }

    p = p_cursor;
  }

  this = p;

  this->next = target;
  return target;
}

struct json_common *
json_add_member(struct json_object *this, struct json_common *target) {
  if(this == NULL || target == NULL)
    return NULL;

  if(this->member == NULL) {
    this->member = target;
  } else {
    struct json_common *jm = _get_last_value(this->member);
    json_add(jm, target);
  }

  return target;
}


struct json_common *
json_array_add_item(struct json_array *this, struct json_common *target) {
  struct json_common *p, *p_cursor;


  if(this == NULL || target == NULL) {
    return NULL;
  }

  p_cursor = this->items;
  if(p_cursor == NULL) {
    this->items = target;
  } else {
    for(; p_cursor != NULL; p_cursor = p_cursor->next) {
      p = p_cursor;
    }

    p->next = target;
  }

  return target;
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

struct json_array *
json_array_new(const char *key) {
  struct json_array *p;

  
  if(key != NULL && strlen(key) < 1) {
    //fprintf(stderr, "");
    return NULL;
  }

  p = (struct json_array *)calloc(1, sizeof(struct json_array));

  strncpy(p->key, key, __JSON_KEY_SIZE);

  p->type = ARRAY;
  p->size = 0;
  p->next = NULL;
  p->add  = json_add;

  return p;
}


struct json_array *
json_array_add(struct json_array *this, struct json_common *target) {
  struct json_common *p_cursor = NULL, *p_next = NULL;


  if(this == NULL || target == NULL) {
    return this;
  }

  if(this->items != NULL) {
    for(p_cursor = this->items; p_cursor != NULL; p_cursor = p_cursor->next) {
      p_next = p_cursor;
    }

    p_next->next = target;
  } else {
    this->items = target;
  }

  return this;
}


struct json_common *
json_common_alloc(const int type, const size_t size, const char *key) {
  struct json_common *jc = NULL;

  switch(type) {
    case STRING:
      jc = _json_alloc(struct json_string);
      break;

    case NUMBER:
      jc = _json_alloc(struct json_number);
      break;

    case DOUBLE:
      jc = _json_alloc(struct json_double);
      break;

    case OBJECT:
      jc = _json_alloc(struct json_object);
      break;

    case ARRAY:
      jc = _json_alloc(struct json_array);
      break;

    case BOOLEAN:
      jc = _json_alloc(struct json_boolean);
      break;

    case NIL:
      jc = _json_alloc(struct json_common);
      break;

    default:
      // fprintf(stderr, "");
      break;
  }

  if(jc == NULL) {
    // fprintf(stderr, "");
    goto end;
  }

  jc->type = type;
  jc->size = size;
  strncpy((char *)jc->key, key, __JSON_KEY_SIZE);

  jc->add  = json_add;
  jc->next = NULL;

end:
  return jc;
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

  jvalue = json_common_alloc(type, size, key);
  _throw(jvalue, error);

  switch(type) {
    case STRING:
      jvalue->size = sizeof(char *) * strlen((const char *)value);
      _JSON_PTR(string, jvalue)->value = (char *)calloc(jvalue->size, sizeof(char));
      memmove(_JSON_PTR(string, jvalue)->value, value, jvalue->size);
      break;

    case BOOLEAN:
      jvalue->size = sizeof(bool_t);
      memcpy((void *)&_JSON_PTR(number, jvalue)->value, value, sizeof(bool_t));
      break;

    case NUMBER:
      jvalue->size = sizeof(long);
      memcpy((void *)&_JSON_PTR(number, jvalue)->value, value, sizeof(long));
      //memmove(jvalue->value, value, sizeof(long));
      break;

    case DOUBLE:
      jvalue->size = sizeof(double);
      memcpy((void *)&_JSON_PTR(double, jvalue)->value, value, sizeof(double));
      //memmove(jvalue->value, value, sizeof(double));
      break;

    case ARRAY:
      jvalue->size = 1;
      _JSON_PTR(array, jvalue)->items = NULL;
      break;

    case NIL:
      jvalue->type  = NIL;
      jvalue->size  = 0;
      break;

    default:
      _throw(NULL, error);
  }

  //

  return jvalue;

error:
  return NULL;
}

struct json_number *
json_number_new(const char *key, const long value) {
  return (struct json_number *)json_value_new(NUMBER, sizeof(long), key, (void *)&value);
}

struct json_double *
json_double_new(const char *key, const double value) {
  return (struct json_double *)json_value_new(DOUBLE, sizeof(double), key, (void *)&value);
}

struct json_boolean *
json_boolean_new(const char *key, const bool_t value) {
  return (struct json_boolean *)json_value_new(BOOLEAN, sizeof(bool_t), key, (void *)&value);
}

struct json_string *
json_string_new(const char *key, const char *value) {
  return (struct json_string *)json_value_new(STRING, sizeof(char) * strlen(value), key, (void *)value);
}


struct json_common *
json_value_free(struct json_common *p) {
  // この関数では object, array は開放しない.
  if(p == NULL || p->type == OBJECT || p->type == ARRAY) {
    return p;
  }

  if(p->type == STRING) {
    struct json_string *p_str = NULL;

    p_str = _JSON_PTR(string, p);

    memset((void *)p_str->value, '\0', p_str->size);
    free((void *)p_str->value);
    p_str->value = NULL;
  }

  free((void *)p);
  p = NULL;

  return NULL;
}


void
json_array_free(struct json_array *jarr) {
  struct json_common *p, *p_next;


  if(jarr == NULL || jarr->items == NULL) {
    //fprintf(stderr, "");
    return;
  }

  p = jarr->items;
  while(p != NULL) {
    p_next = p->next;

    switch(p->type) {
      case OBJECT:
        json_object_free((struct json_object *)p);
        break;

      case ARRAY:
        json_array_free((struct json_array *)p);
        break;

      default:
        json_value_free(p);
        break;
    }

    p = p_next;
  }
}


void
json_object_free(struct json_object *jo) {
  struct json_common *p      = NULL,
                     *p_next = NULL;


  if(jo == NULL)
    return;

  p = jo->member;

  while(p != NULL) {
    p_next = p->next;

    switch(p->type) {
      case OBJECT:
        json_object_free((struct json_object *)p);
        break;

      case ARRAY:
        break;

      default:
        json_value_free(p);
        break;
    }

    p = p_next;
  }

  free((void *)jo->member);
  jo->member = NULL;

  free((void *)jo);
  jo = NULL;
}


size_t
json_object_count(const struct json_object *jo) {
  size_t count;
  struct json_common *p = NULL;


  if(jo == NULL || jo->member == NULL)
    return -1;

  p      = jo->member;
  count = 0L;
  for(; p != NULL; p = p->next) {
    ++count;
  }

  return count;
}

size_t
json_array_count(const struct json_array *ja) {
  size_t count;
  struct json_common *p;

  if(ja == NULL || ja->items == NULL) {
    return -1;
  }

  p     = ja->items;
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
