#ifndef _L_S_H
#define _L_S_H

typedef struct string string_t;

struct string {
  size_t size;
  char *string;

  string_t *next;
};


/* 対象の文字列を指定した文字で区切 */
extern string_t *
string_split(const char *str, /* 対象文字列 */
    const char *sep           /* 区切り文字 */);

/* 割り当てられた string_t の解放 */
extern void
string_free(string_t *this /* 対象の先頭アドレス */);

/* 対象文字列から指定された文字列の削除 */
extern char *
string_delete(char *this, /* 対象文字列 */
    const char *target    /* 削除文字 */);

/* string_t のデータ数を取得 */
extern size_t
string_count(const string_t *this /* 対象 */);

/**/
extern char **
string_to_array(const string_t *this, /* 対象 */
    size_t *size                      /* データサイズ */);


/* 対象文字列から指定された文字列の削除
   戻り値は新規に割り当てられる.
 */
extern char *
string_delete_new(const char *this, /* 対象文字列 */
    const char *target              /*削除文字*/);

#endif
