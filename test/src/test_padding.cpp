#include <test_padding.h>
#include <padding.h>

TEST_F(FeilstelXorPaddingTest, PadPKCS7)
{
  unsigned char* data = (unsigned char*)malloc(32 /* buf_length */);

  int buf_length = 32;
  int pad_length = 8;
  PAD_PKCS7(data, buf_length, pad_length);

  int buf_tail = buf_length - pad_length;

  for (int i = buf_tail; i < buf_length; i++) {
    ASSERT_EQ(data[i], pad_length);
  }
}


TEST_F(FeilstelXorPaddingTest, DepadPKCS7)
{
  unsigned char data[32] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
  };

  int pad_length = GET_PADDED_SIZE(data, 32);

  ASSERT_EQ(pad_length, 8);
}
