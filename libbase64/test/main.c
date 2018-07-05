#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "libbase64.h"


void
test_encode(char *str) {
  int status;
  base64_t b64;

  memset((void *)&b64, '\0', sizeof(base64_t));

  b64.src = str;
#if 0
  b64.src = (char *)calloc(strlen(str) + 1, sizeof(char));
  strncpy(b64.src, (const char *)str, strlen(str));
#endif

  if((status = base64_encode(&b64)) == BASE64_OK) {
    fprintf(stderr, "*** Base64 Encode ***\n");
    fprintf(stderr, "\t%s -> %s\n\n", b64.src, b64.dest);

    base64_free(&b64);
  }
}

void
test_encode_file(const char *path) {
  int fd, status;
  base64_t b64;


  memset((void *)&b64, '\0', sizeof(base64_t));

  if((fd = open(path, O_RDONLY)) > 0) {
    off_t _size;
    char *_ctx;


    _size = lseek(fd, 0L, SEEK_END);
    lseek(fd, 0L, SEEK_SET);

    _ctx = (char *)calloc(_size, sizeof(char));
    read(fd, (void *)_ctx, sizeof(char) * _size);


    b64.src = _ctx;

    if((status = base64_encode(&b64)) == BASE64_OK) {
      fprintf(stderr, "*** Base64 Encode(%s) ***\n", path);

      int output_fd;
      if((output_fd = open("output.txt", O_WRONLY | O_CREAT, 0644)) > 0) {
        write(output_fd, (const void *)b64.dest, sizeof(char) * strlen(b64.dest));
        close(output_fd);
      }

      fprintf(stderr, "%s\n", b64.dest);
      fprintf(stderr, "\tEncode Length: %ld\n", b64.dest_length);
      base64_free(&b64);
    }

    memset((void *)_ctx, '\0', sizeof(char) * _size);
    free((void *)_ctx);
    _ctx = NULL;

    close(fd);
  }
}

void
test_decode(char *str) {
  int status;
  base64_t b64;

  memset((void *)&b64, '\0', sizeof(base64_t));
  b64.src = str;

  if((status = base64_decode(&b64)) == BASE64_OK) {
    fprintf(stderr, "*** Base64 Decode ***\n");
    fprintf(stderr, "\t%s -> %s\n\n", b64.src, b64.dest);

    base64_free(&b64);
  }
}

int
main(int argc, char *argv[]) {
  test_encode("Hello, Worl");
  test_decode("SGVsbG8sIFdvcmxkIQ==");
  test_decode("SGVsbG8sIFdvcmw=");

  test_encode_file("random.txt");
  return 0;
}
