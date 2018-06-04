#ifndef _L_SB_H
#define _L_SB_H


struct string_builder {
  size_t size;
  char   *buf;

  struct string_builder *next;
};


/**
 * 新規 struct string_builder * の割当
 *
 * > void
 * < struct string_builder * */
extern struct string_builder *
string_builder_new(void);

/**
 * struct string_builder * の解放
 *
 * > struct string_builder *this: 対象ポインタ
 * < void */
extern void
string_builder_free(struct string_builder *this);


/**
 * struct string_builder * 追加
 *
 * > struct string_builder *this: 追加対象
 * > const char *str: 追加する文字列
 * < struct string_builder *: 追加した struct string_builder のポインタ */
extern struct string_builder *
string_builder_append(struct string_builder *this,
    const char *str);

/**
 * 現在のサイズを取得
 *
 * > const struct string_builder *this: 対象
 * < size_t */
extern size_t
string_builder_length(const struct string_builder *this);

// NOTE: 解放(free) が必要
/**
 * 格納された文字列データを結合して出力
 *
 * > const struct string_builder *this
 * < char *: 新規に割り当てられた char * */
extern char *
string_builder_to_string(const struct string_builder *this);

#endif
