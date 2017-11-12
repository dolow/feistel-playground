#ifndef FEISTELXOR_PADDING_H
#define FEISTELXOR_PADDING_H

#ifndef BARED_SYMBOL
  #define PAD_PKCS7 align_texture
  #define GET_PADDED_SIZE get_alignment_size
#else
  #define PAD_PKCS7 pad_pkcs7
  #define GET_PADDED_SIZE get_padded_size
#endif

void PAD_PKCS7(unsigned char* buf, int buf_length, int pad_length);
int GET_PADDED_SIZE(unsigned char* buf, int buf_length);

#endif // FEISTELXOR_PADDING_H
