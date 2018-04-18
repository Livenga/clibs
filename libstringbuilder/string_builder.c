#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "libstringbuilder.h"

#define _sballoc() \
  (struct string_builder *)calloc(1, sizeof(struct string_builder))


/** エラー出力
 */
static int eprintf(FILE *strm,
    const char *fn,
    const char *param) {
  if(param != NULL) {
    return fprintf(strm, "%s: %s %s\n", fn, param, strerror(errno));
  }
  return fprintf(strm, "%s: %s\n", fn, strerror(errno));
}


/** 連続した (struct string_builder *)->next の最後のポインタを取得
 * > struct string_builder *this: 検索対象
 * < struct string_builder *
 */
static struct string_builder *
_last_pointer(struct string_builder *this) {
  struct string_builder *sb = NULL,
  *cursor = NULL;

  if(this != NULL) {
    for(sb = this; sb != NULL; sb = sb->next) {
      //fprintf(stderr, "sb: %p\n", sb);
      cursor = sb;
    }

    sb = cursor;
  }

  return sb;
}


/**
 * 新規 struct string_builder * の割当
 *
 * > void
 * < struct string_builder * */
struct string_builder *
string_builder_new(void) {
  struct string_builder *sb = NULL;

  if((sb = _sballoc()) != NULL) {
    sb->next   = NULL;
    sb->append = string_builder_append;
  }

  return sb;
}

/**
 * struct string_builder * の解放
 *
 * > struct string_builder *this: 対象ポインタ
 * < void */
void
string_builder_free(struct string_builder *this) {
  struct string_builder *sb = this, *next = NULL;

  while(sb != NULL) {
    next = sb->next;

    if(sb->buf != NULL) {
      memset((void *)sb->buf, '\0', strlen(sb->buf));
      //memset((void *)sb->buf, '\0', sb->size);

      sb->size   = 0;
      sb->append = NULL;
      free((void *)sb->buf);
    }
    free((void *)sb);

    sb = next;
  }
}

/**
 * struct string_builder * 追加
 *
 * > struct string_builder *this: 追加対象
 * > const char *str: 追加する文字列
 * < struct string_builder *: 追加した struct string_builder のポインタ
 */
struct string_builder *
string_builder_append(struct string_builder *this,
    const char *str) {
  const size_t new_size = strlen(str);
  struct string_builder *new = NULL, *sb = NULL;


  if(this == NULL || new_size == 0)
    goto end;

  if((new = string_builder_new()) == NULL) {
    eprintf(stderr, "string_builder_new(3)", NULL);
    // エラー
    return this;
  }

  sb          = _last_pointer(this);
  sb->next    = new;

  new->next   = NULL;
  new->size   = new_size;
  if((new->buf = (char *)calloc(new_size, sizeof(char))) == NULL) {
    new->size = -1;
    new->buf  = NULL;
    goto end;
  }

  strncpy(new->buf, str, new_size);

end:
  return new;
}


/**
 * 現在のサイズを取得
 *
 * > const struct string_builder *this: 対象
 * < size_t
 */
size_t
string_builder_length(const struct string_builder *this) {
  const struct string_builder *sb = this;
  size_t size;


  if(sb == NULL) return -1;

  size = 0L;
  for(; sb != NULL; sb = sb->next) {
    if(sb->size >= 0)
      size += sb->size;
  }

  return size;
}

// NOTE: 解放(free) が必要
/**
 * 格納された文字列データを結合して出力
 *
 * > const struct string_builder *this
 * < char *: 新規に割り当てられた char * */
char *
tring_builder_to_string(const struct string_builder *this) {
  const struct string_builder *sb;

  size_t size;
  char *buf = NULL;


  if(this == NULL) return NULL;

  size = string_builder_length(this);

  if((buf = (char *)calloc(size, sizeof(char))) == NULL) {
    eprintf(stderr, "calloc(3)", NULL);
    goto end;
  }

  for(sb = this; sb != NULL; sb = sb->next) {
    strncat(buf, sb->buf, sb->size);
  }


end:
  return buf;
}
