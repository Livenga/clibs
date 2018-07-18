#ifndef _LIB_HTML_H
#define _LIB_HTML_H

typedef struct file_t      html_file_t;
typedef struct tag_t       html_tag_t;
typedef struct attribute_t html_attribute_t;


struct file_t {
  char   *path;

  size_t size;
  char   *content;
};

struct attribute_t {
  size_t name_size;
  char   *name;

  size_t value_size;
  char   *value;

  html_attribute_t *next;
};


struct tag_t {
  char *name;
  html_attribute_t *attribute;
  char *text;
  html_tag_t *next;
};

/* attribute.c */
extern html_attribute_t *
html_attribute_parse(char *p_start, char *p_end);

extern void
html_attribute_free(html_attribute_t *this);

/* html.c */
extern void
html_parse(const char *path);

#endif
