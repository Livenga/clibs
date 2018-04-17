#ifndef _L_SB_H
#define _L_SB_H


struct string_builder {
  size_t size;
  char   *buf;
  struct string_builder *next;

  struct string_builder *(*append)(struct string_builder *, const char *);
};

extern struct string_builder *
string_builder_new(void);
extern void
string_builder_free(struct string_builder *this);


extern struct string_builder *
string_builder_append(struct string_builder *this,
    const char *str);

extern size_t
string_builder_length(const struct string_builder *this);

extern char *
string_builder_to_string(const struct string_builder *this);

#endif
