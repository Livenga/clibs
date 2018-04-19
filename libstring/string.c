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

  str->size   = 0; str->string = NULL;
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
      str_new->size = p_cursor - p;
    } else {
      str_new->size = sizeof(char) * _len - (p - buf);
      is_break      = 1;
    }

    str_new->string = (char *)malloc(str_new->size);
    if(str_new->string == NULL) {
      free((void *)str_new);
      break;
    }
    memset((void *)str_new->string, '\0', str_new->size);
    memmove((void *)str_new->string, p, str_new->size);

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


/* 解放 */
void
string_free(string_t *this /* 対象の先頭アドレス */) {
  string_t *p, *p_next;


  if(this == NULL) { return; }

  p = this;
  while(p != NULL) {
    p_next = p->next;

    if(p->string != NULL) {
      memset((void *)p->string, '\0', p->size);
      free((void *)p->string); p->string = NULL;
    }
    p->size   = 0L;
    p->string = NULL;
    p->next   = NULL;
    free((void *)p);

    p = p_next;
  }
}
