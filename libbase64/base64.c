#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "libbase64.h"


/**
 * 整数値をBase64 変換表の文字に変換
 */
static char
_deg2char(int deg) {
  if(deg >= 0 && deg < 26) {
    return 'A' + deg;
  } else if(deg >= 26 && deg < 52) {
    return 'a' + (deg - 26);
  } else if(deg >= 52 && deg < 62) {
    return '0' + (deg - 52);
  } else if(deg == 62) {
    return '+';
  } else {
    return '/';
  }
}

/**
 * Base64 文字を変換表の整数値に変換
 */
static char
_char2deg(char chr) {
  if(chr >= 'A' && chr <= 'Z') {
    return chr - 'A';
  } else if(chr >= 'a' && chr <= 'z') {
    return 26 + chr - 'a';
  } else if(chr >= '0' && chr <= '9') {
    return 52 + chr - '0';
  } else if(chr == '+') {
    return 62;
  } else {
    return 63;
  }
}


/**
 * Base64 にエンコードする.
 * @param base64_t * this
 * @return エラーコード
 */
int
base64_encode(base64_t *this) {
  int i, j, cursor;


  cursor = 0;
  if(this == NULL || this->src == NULL) {
    return BASE64_NULL;
  }

  // 元データの長さを取得
  // '\0' を含ませないため strlen を使用
  this->src_length = strlen(this->src);

  // ビット数の取得
  size_t        num_bits, n_rest;
  unsigned long value;

  // 必要なビット数を取得
  num_bits   = (this->src_length * 8);
  if((n_rest = num_bits % 6) != 0) {
    num_bits += (6 - n_rest);
  }

  // 4文字で区切るため, 4の倍数に調整
  this->dest_length = (num_bits / 6) + (4 - (num_bits / 6) % 4);
  this->dest        = (char *)calloc(this->dest_length + 1, sizeof(char));
  if(this->dest == NULL) {
    return BASE64_ERROR_ALLOC;
  }

#ifdef __DEBUG__
  fprintf(stderr, "* Source Length    : %ld\n", this->src_length);
  fprintf(stderr, "* Dest Length      : %ld\n", this->dest_length);
  fprintf(stderr, "* Number of bits   : %ld\n",   num_bits);
  fprintf(stderr, "* Number of Base64 : %ld\n\n", num_bits / 6);
#endif

  char bit6;
  for(i = 0; i < this->src_length / 3; ++i) {
    value = (this->src[i * 3 + 0] << 16)
      + (this->src[i * 3 + 1] << 8)
      + (this->src[i * 3 + 2] << 0);

    for(j = 3; j > -1; --j) {
      bit6  = (value >> (j * 6)) & 0b111111;
      *(this->dest + cursor++) = _deg2char(bit6);
    }
  }
  if(this->src_length % 3 != 0) {
    int   _len, _n_equals;


    _n_equals = 0;
    _len      = this->src_length - ((this->src + i * 3) - this->src);
    value     = 0L;
    memcpy((void *)&value, (const void *)(this->src + i * 3), _len);

    switch(_len) {
      case 1:
        _n_equals = 2;
        value     = (*(this->src + i * 3));
        value   <<= 4;
        break;

      case 2:
        _n_equals = 1;
        value     = (*(this->src + (i * 3 + 0)) << 8) + (*(this->src + (i * 3 + 1)) << 0);
        value   <<= 2;
        break;
    }

    for(i = _len; i >= 0; --i) {
      bit6 = (value >> (i * 6)) & 0b111111;
      *(this->dest + cursor++) = _deg2char(bit6);
    }
    for(i = 0; i < _n_equals; ++i) {
      *(this->dest + cursor++) = '=';
    }
  }

  return BASE64_OK;
}

/**
 * Base64 をデコード
 * @param base64_t * this
 * @return エラーコード
 */
int
base64_decode(base64_t *this) {
  int  i, j, cnt;
  char *p_equal;


  if(this == NULL || this->src == NULL) {
    return BASE64_NULL;
  }
  if(this->dest_length != 0 || this->dest != NULL) {
    return BASE64_ERROR_EXISTS;
  }
  if(strlen(this->src) % 4 != 0) {
    return BASE64_ERROR_STRING;
  }

  this->src_length = strlen(this->src);
  if((p_equal = strchr((const char *)this->src, '=')) != NULL) {
    this->src_length = (p_equal - this->src);
  }


  int  n_rest;
  long value;

  n_rest = this->src_length % 4;

  this->dest_length = (this->src_length / 4) * 3 + ((n_rest == 0) ? 0 : (1 + n_rest % 2));
  this->dest        = (char *)calloc(this->dest_length + 1, sizeof(char));
  if(this->dest == NULL) {
    return BASE64_ERROR_ALLOC;
  }

#if __DEBUG__
  fprintf(stderr, "* Decode Output length: %ld\n", this->dest_length);
#endif

  cnt = 0;
  for(i = 0; i < this->src_length / 4; ++i) {
    value = 0L;

    for(j = 0; j < 4; ++j) {
      value += _char2deg(this->src[i * 4 + j]) << (6 * (3 - j));
    }

    for(j = 0; j < 3; ++j) {
      *(this->dest + cnt++) = (char)(value >> (8 * (2 - j))) & 0xFF;
    }
  }
  if(n_rest  != 0) {
    size_t _n_loop;

    value = 0L;
    for(j = 0; j < n_rest; ++j) {
      value += _char2deg(*(this->src + ((i * 4) + j))) << (6 * (n_rest - j - 1));
    }

    switch(n_rest) {
      case 2: // 1
        value >>= 4;
        break;

      case 3: // 2
        value >>= 2;
        break;
    }
    _n_loop = 1 + (n_rest % 2);

    for(j = 0; j < _n_loop; ++j) {
      *(this->dest + cnt++) = (char)((value >> (8 * (_n_loop - j - 1))) & 0xFF);
    }
  }


  return BASE64_OK;
}

#define _XFREE(ptr, v_len) \
  v_len = strlen(ptr); \
  memset((void *)ptr, '\0', v_len * sizeof(char)); \
  free((void *)ptr)


// NOTE: (base64_t)->src に対して解放処理を行わない.
/**
 * (base64_t)->dest に割り当てられた領域の解放.
 * @param base64_t * this
 */
void
base64_free(base64_t *this) {
  size_t len;

#if 0
  if(this->src != NULL) {
    _XFREE(this->src, len);
    this->src = NULL;
  }
#endif

  if(this->dest != NULL) {
    _XFREE(this->dest, len);
    this->dest = NULL;
  }
}
