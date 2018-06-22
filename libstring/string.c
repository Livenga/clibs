#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "libstring.h"


//static int error_code = 0;

#define _string_alloc() \
  (string_t *)calloc(1, sizeof(string_t))

/* string_t 領域の割当
 */
static string_t *
string_new(void) {
  string_t *str;

  if((str = _string_alloc()) == NULL) {
    return NULL;
  }

  str->length = 0;
  str->string = NULL;
  str->next   = NULL;

  return str;
}


/* 対象の文字列を指定した文字で区切る */
string_t *
string_split(const char *str, /* 対象文字列 */
    const char *sep           /* 区切り文字 */) {
  const size_t _len = strlen(str),
        _sep_len = strlen(sep);

  short is_break = 0;
  char *buf, *p, *p_cursor;
  string_t *str_new, *str_root = NULL, *str_next = NULL;


  if((buf = (char *)calloc(_len, sizeof(char))) == NULL) {
    return NULL;
  }
  strncpy(buf, str, _len);

  p = buf;
  do {
    if(p != buf) {
      p += _sep_len;
    }

    if((str_new = string_new()) == NULL) { break; }

    if((p_cursor = strstr(p, sep)) != NULL) {
      str_new->length = p_cursor - p;
    } else {
      str_new->length = sizeof(char) * _len - (p - buf);
      is_break      = 1;
    }

    str_new->string = (char *)malloc(str_new->length);
    if(str_new->string == NULL) {
      free((void *)str_new);
      break;
    }
    memset((void *)str_new->string, '\0', str_new->length);
    memmove((void *)str_new->string, p, str_new->length);

    if(str_root == NULL) {
      str_root = str_new;
      str_next = str_root;
    } else {
      str_next->next = str_new;
      str_next       = str_new;
    }

    if(is_break) { break; }
  } while((p = strstr(p, sep)) != NULL);

  memset((void *)buf, '\0', sizeof(char) * _len);
  free((void *)buf);
  buf = NULL;

  return str_root;
}


/* 対象文字列から指定された文字列の削除 */
char *
string_delete(char *this, /* 対象文字列 */
    const char *target    /* 削除文字 */) {
  const size_t _len = strlen(this),
        _len_target = strlen(target);

  char *p, *p_next;

  p = this;
  while((p_next = strstr(p, target)) != NULL) {
    *p_next = '\0';
    strncat(p, p_next + _len_target, _len);
  }

  return this;
}


/* 対象文字列から指定された文字列の削除
   戻り値は新規に割り当てられる.
 */
char *
string_delete_new(const char *this, /* 対象文字列 */
    const char *target              /*削除文字*/) {
  const size_t len = strlen(this);

  char *p_new = NULL;
  if((p_new = (char *)calloc(len, sizeof(char))) == NULL) { return NULL; }
  strncpy(p_new, this, len);

  return string_delete(p_new, target);
}


/* string_t のデータ数を取得 */
size_t
string_count(const string_t *this /* 対象 */) {
  size_t size;
  string_t *p_cursor;


  if(this == NULL) { return -1; }

  size = 0;
  for(p_cursor = (string_t *)this; p_cursor != NULL; p_cursor = p_cursor->next) {
    ++size;
  }

  return size;
}

/**/
char **
string_to_array(const string_t *this, /* 対象 */
    size_t *size                      /* データサイズ */) {
  const string_t *p_cursor;

  size_t i, count;
  char **p_ret;


  if(this == NULL) {
    return NULL;
  }

  count = string_count(this);
  if(size != NULL) { *size = count; }

  p_ret = (char **)calloc(count, sizeof(char *));

  for(p_cursor = this, i = 0; p_cursor != NULL; p_cursor = p_cursor->next, ++i) {
    *(p_ret + i) = p_cursor->string;
  }

  return p_ret;
}

/* 解放 */
void
string_free(string_t *this /* 対象の先頭アドレス */) {
  string_t *p, *p_next;


  if(this == NULL) { return; }

  p = this;
  while(p != NULL) {
    p_next = p->next;

    if(p->string != NULL) {
      memset((void *)p->string, '\0', p->length);
      free((void *)p->string); p->string = NULL;
    }
    p->length   = 0L;
    p->string = NULL;
    p->next   = NULL;
    free((void *)p);

    p = p_next;
  }
}


/**
 *
 */
string_builder_t *
string_builder_new(const char *str) {
  string_builder_t *sb;

  sb = NULL;
  if(str == NULL) {
    return NULL;
  }

  sb = (string_builder_t *)calloc(1, sizeof(string_builder_t));
  if(sb == NULL) {
    return sb;
  }

  sb->length = strlen(str);
  sb->data = (char *)calloc(sb->length + 1, sizeof(char));
  sb->next = NULL;

  if(sb->data == NULL) {
    memset((void *)sb, '\0', sizeof(string_builder_t));
    free((void *)sb);
    sb = NULL;
  }

  strncpy(sb->data, str, sb->length);

  return sb;
}

string_builder_t *
string_builder_add(string_builder_t *this, const char *str) {
  string_builder_t *sb, *target;


  if(str == NULL) {
    return this;
  }

  if(this->next != NULL) {
    for(sb = this; sb != NULL; sb = sb->next) {
      target = sb;
    }
  } else {
    target = this;
  }

  sb = string_builder_new(str);
  if(sb == NULL) {
    return NULL;
  }

  target->next = sb;
  return sb;
}


void
string_builder_free(string_builder_t *this) {
  string_builder_t *next;


  if(this == NULL) {
    return;
  }

  do {
    next = this->next;

    if(this->data != NULL) {
      memset((void *)this->data, '\0', sizeof(char) * this->length);
      free((void *)this->data);
      this->data = NULL;
    }
    memset((void *)this, '\0', sizeof(string_builder_t));
    free(this);

    this = next;
  } while(this != NULL);
}


size_t
string_builder_length(const string_builder_t *this) {
  const string_builder_t *next;
  size_t length;

  length = 0;
  for(next = this; next != NULL; next = next->next) {
    if(next->data != NULL) {
#if 1
      length += next->length;
#else
      length += strlen(next->data);
#endif
    }
  }

  return length;
}


string_t *
string_builder_to_string(const string_builder_t *this) {
  const string_builder_t *next;
  string_t *ret;


  if(this == NULL) {
    return NULL;
  }

  ret = string_new();
  if(ret == NULL) {
    return NULL;
  }

  ret->length = string_builder_length(this);

  ret->string = (char *)calloc(ret->length + 1, sizeof(char));
  if(ret->string == NULL) {
    memset((void *)ret, '\0', sizeof(string_t));
    free((void *)ret);
    ret = NULL;

    return NULL;
  }


  size_t l_cur;
  for(l_cur = 0, next = this; next != NULL; next = next->next) {
    if(next->length <= 0 || next->data == NULL) {
      continue;
    }

    strncpy(ret->string + l_cur, (const char *)next->data, next->length);

    l_cur += next->length;
  }

  return ret;
}
