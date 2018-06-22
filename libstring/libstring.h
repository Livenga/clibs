#ifndef _L_S_H
#define _L_S_H

typedef struct _string_t         string_t;
typedef struct _string_builder_t string_builder_t;

struct _string_t {
  size_t length;
  char   *string;

  string_t *next;
};

struct _string_builder_t {
  size_t length;
  char   *data;

  string_builder_t *next;
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



extern string_builder_t *
string_builder_new(const char *str);

extern string_builder_t *
string_builder_add(string_builder_t *this, const char *str);

extern void
string_builder_free(string_builder_t *this);

extern size_t
string_builder_length(const string_builder_t *this);
extern string_t *
string_builder_to_string(const string_builder_t *this);

#endif
