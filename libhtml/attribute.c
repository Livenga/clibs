#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "libhtml.h"

//
// ``` html_attribute_t *html_attribute_parse(char *, char *)
html_attribute_t *
html_attribute_parse(char *p_start, char *p_end) {
  char *p_space, *p_equal, *ps_dq, *pe_dq;
  html_attribute_t *attr_rootptr, *attr_preptr;


  if(p_start == NULL || p_end == NULL) {
    return NULL;
  }

  attr_rootptr = NULL;
  attr_preptr  = NULL;
  while(1) {
    int _is_break;
    char *_p_end;

    html_attribute_t *_attr_ptr;


    for(; *p_start == ' '; ++p_start);
    p_space = strchr(p_start, ' ');
    p_equal = strchr(p_start, '=');


    if(p_space < p_equal) {
      p_equal = NULL;
    } else if(p_space != NULL && *(p_equal + 1) == '"') {
      // class="class1 class2"
      // 上記のように要素値の中にスペース' ' が含まれている場合,
      // イコール文字の後のDouble Quotation'"' に囲まれていない外側の
      // 最も違いスペース' ' を取得
      if((ps_dq = strchr(p_equal, '"')) != NULL
          && (pe_dq = strchr(ps_dq + 1, '"')) != NULL) {
        for(; p_space < pe_dq; p_space = strchr(p_space + 1, ' '));
      }
    }

    if(p_space > p_end) {
      _p_end = p_end;
      _is_break = 1;
    } else {
      _p_end = p_space;
      _is_break = 0;
    }

    _attr_ptr = (html_attribute_t *)calloc(1, sizeof(html_attribute_t));
    if(_attr_ptr == NULL) {
      break;
    }
    _attr_ptr->name  = NULL;
    _attr_ptr->value = NULL;
    _attr_ptr->next  = NULL;


    if(attr_rootptr == NULL) {
      attr_rootptr = _attr_ptr;
    } else {
      attr_preptr->next = _attr_ptr;
    }
    attr_preptr = _attr_ptr;

    //
    if(p_equal != NULL) {
      // 特性の名称取得
      _attr_ptr->name_size = p_equal - p_start;
      _attr_ptr->name      = (char *)calloc(_attr_ptr->name_size + 1, sizeof(char));
      if(_attr_ptr->name == NULL) {
        break;
      }
      memcpy((void *)_attr_ptr->name, (const void *)p_start,
          sizeof(char) * _attr_ptr->name_size);

      // 特性の値取得
      ++p_equal;
      _attr_ptr->value_size = _p_end - p_equal;
      _attr_ptr->value      = (char *)calloc(_attr_ptr->value_size + 1, sizeof(char));
      if(_attr_ptr->value == NULL) {
        break;
      }
      memcpy((void *)_attr_ptr->value, (const void *)p_equal,
          sizeof(char) * _attr_ptr->value_size);
    } else {
      // 特性の名称取得
      _attr_ptr->name_size = _p_end - p_start;
      _attr_ptr->name      = (char *)calloc(_attr_ptr->name_size + 1, sizeof(char));
      if(_attr_ptr == NULL) {
        break;
      }
      memcpy((void *)_attr_ptr->name, (const void *)p_start,
          sizeof(char) * _attr_ptr->name_size);
    }

    if(_is_break) {
      break;
    }

    p_start = _p_end;
  }


#if __DEBUG__
  html_attribute_t *_p_attr;

  for(_p_attr = attr_rootptr; _p_attr != NULL; _p_attr = _p_attr->next) {
    if(_p_attr->value != NULL) {
      fprintf(stderr, "* Name: %s, Value: %s\n", _p_attr->name, _p_attr->value);
    } else {
      fprintf(stderr, "* Name: %s\n", _p_attr->name);
    }
  }
#endif

  return attr_rootptr;
}
// '''

//
// ``` void html_attribute_free(html_attribute_t *)
void
html_attribute_free(html_attribute_t *this) {
  html_attribute_t *attr_nextptr;


  if(this == NULL) {
    return;
  }

  do {
    attr_nextptr = this->next;

    if(this->name != NULL) {
      memset((void *)this->name, '\0', sizeof(char) * this->name_size);
      free((void *)this->name);
      this->name = NULL;
    }
    
    if(this->value != NULL) {
      memset((void *)this->value, '\0', sizeof(char) * this->value_size);
      free((void *)this->value);
      this->value = NULL;
    }

    memset((void *)this, '\0', sizeof(html_attribute_t));
    free((void *)this);

    this = attr_nextptr;
  } while(this != NULL);
}
// '''
