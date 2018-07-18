#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "libhtml.h"

typedef struct html_tag_stack_t html_tag_stack_t;

struct html_tag_stack_t {
  char *name;
  char *start, *end;

  html_tag_stack_t *next;
};


//
// ``` static html_file_t *html_file_new(const char *)
static html_file_t *
html_file_new(const char *path) {
  FILE   *fp;
  size_t size;
  html_file_t *file_ptr;


  if((fp = fopen(path, "r")) == NULL) {
    return NULL;
  }

  file_ptr = (html_file_t *)calloc(1, sizeof(html_file_t));
  if(file_ptr == NULL) {
    fclose(fp);
    return NULL;
  }

  // ファイルパス複製
  size           = strlen(path);
  file_ptr->path = (char *)calloc(size + 1, sizeof(char));
  memcpy((void *)file_ptr->path, (const void *)path, sizeof(char) * size);

  // サイズ取得
  fseek(fp, 0L, SEEK_END);
  file_ptr->size = ftell(fp);
  fseek(fp, 0L, SEEK_SET);

  // 読み込み
  file_ptr->content = (char *)calloc(file_ptr->size + 1, sizeof(char));
  size_t sz_read, sz_total;
  
  sz_total = 0;
  while((sz_read = fread((void *)file_ptr->content + sz_total, sizeof(char),
          file_ptr->size - sz_total, fp)) > 0) {
    sz_total += sz_read;
  }


  fclose(fp);
  return file_ptr;
}
// '''

//
// ``` static void html_file_free(html_file_t *)
static void
html_file_free(html_file_t *this) {
  size_t size;


  if(this == NULL) {
    return;
  }

  if(this->path != NULL) {
    size = strlen(this->path);
    memset((void *)this->path, '\0', sizeof(char) * size);
    free((void *)this->path);
  }

  if(this->content != NULL) {
    memset((void *)this->content, '\0', sizeof(char) * this->size);
    free((void *)this->content);
  }

  memset((void *)this, '\0', sizeof(html_file_t));
  free((void *)this);
  this = NULL;
}
// '''


//
static void
_html_parse(char *html_str) {
  char tag[64];
  char *p_html, *p_lt, *p_gt;


  p_html = html_str;
  while(1) {
    char *_p_space, *_p_name;


    memset((void *)tag, '\0', sizeof(tag));
    if((p_lt = strchr(p_html, '<')) == NULL || (p_gt = strchr(p_lt, '>')) == NULL) {
      break;
    }

    _p_space = strchr(p_lt, ' ');
    if(_p_space != NULL) {
      _p_name = (_p_space > p_gt) ? p_gt : _p_space;
    } else {
      _p_name = p_gt;
    }
    memcpy((void *)tag, (const void *)p_lt + 1, sizeof(char) * (_p_name - p_lt - 1));

    if(*(p_lt + 1) != '/') {
      html_attribute_t *_attr;
      _attr = html_attribute_parse(p_lt + 1, p_gt);
      html_attribute_free(_attr);
    }


    p_html = p_gt;
  }
}

void
html_parse(const char *path) {
  html_file_t *html_fptr;
  char *ctx;


  if((html_fptr = html_file_new(path)) == NULL) {
    return;
  }

#if __DEBUG__
  fprintf(stderr, "File Path: %s\n", html_fptr->path);
  fprintf(stderr, "File Size: %ld\n", html_fptr->size);
#endif

  ctx  = html_fptr->content;
  _html_parse(ctx);

  html_file_free(html_fptr);
}
