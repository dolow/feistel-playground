#include "padding.h"

#include <stdlib.h>
#include <string.h>

void PAD_PKCS7(unsigned char* buf, int buf_length, int pad_length)
{
  int offset = buf_length - pad_length;
  memset(buf + offset, pad_length, pad_length);
}
int GET_PADDED_SIZE(unsigned char* buf, int buf_length)
{
  return buf[buf_length - 1];
}
