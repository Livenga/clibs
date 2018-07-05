#ifndef _LIB_BASE64_H
#define _LIB_BASE64_H

#define BASE64_OK           +0
#define BASE64_NULL         -1
#define BASE64_ERROR_ALLOC  -2
#define BASE64_ERROR_STRING -3
#define BASE64_ERROR_EXISTS -4


typedef struct base64_t base64_t;

struct base64_t {
  size_t src_length;
  char   *src;

  size_t dest_length;
  char   *dest;
};


/* base64.c */
extern int
base64_encode(base64_t *this);
extern int
base64_decode(base64_t *this);
extern void
base64_free(base64_t *this);

#endif
