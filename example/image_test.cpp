#include <main.h>
#include <padding.h>
#include <stdio.h>

const char* mode_ecb = "ECB";
const char* mode_cbc = "CBC";

void encrypt_and_decrypt_bmp(const char* inputFile, const char* mode)
{
  int header_size = 128;
  unsigned char header[128];

  FILE* fd = fopen(inputFile, "r");
  fseek(fd, 0, SEEK_END);
  long size = ftell(fd);
  fseek(fd, 0, SEEK_SET);

  fread(header, header_size, 1, fd);

  fseek(fd, 0, SEEK_SET);

  int pad_length = size % BLOCK_SIZE + BLOCK_SIZE;

  unsigned char* file_content = (unsigned char*)malloc(size + pad_length);
  fread(file_content, size, 1, fd);
  fclose(fd);

  PAD_PKCS7(file_content, size + pad_length, pad_length);

  unsigned char key[BLOCK_SIZE] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
  };
  unsigned char iv[BLOCK_SIZE] = {
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
  };

  unsigned char* ct = (unsigned char*)malloc(size + pad_length);

  if (strcmp(mode, mode_ecb) == 0) {
    ENCRYPT_ECB(key, file_content, ct, size + pad_length);
  }
  else {
    ENCRYPT_CBC(key, iv, file_content, ct, size + pad_length);
  }

  unsigned char* ct_jpg = (unsigned char*)malloc(size + pad_length);
  memcpy(ct_jpg, ct, size + pad_length);
  memcpy(ct_jpg, header, header_size);

  fd = fopen("encrypt.bmp", "w");

  fwrite(ct_jpg, size + pad_length, 1, fd);
  fclose(fd);

  unsigned char* dt = (unsigned char*)malloc(size + pad_length);

  if (strcmp(mode, mode_ecb) == 0) {
    DECRYPT_ECB(key, ct, dt, size + pad_length);
  }
  else {
    DECRYPT_CBC(key, iv, ct, dt, size + pad_length);
  }

  fd = fopen("decrypt.bmp", "w");

  fwrite(dt, size, 1, fd);
  fclose(fd);
}

int main(int argc, const char** argv)
{
  if (argc <= 2) {
    return 0;
  }

  encrypt_and_decrypt_bmp(argv[1], argv[2]);

  return 0;
}
