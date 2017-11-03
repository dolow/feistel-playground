#include "main.h"

void round_forward(const unsigned char k[HALF_BLOCK_SIZE], unsigned char l[HALF_BLOCK_SIZE], unsigned char r[HALF_BLOCK_SIZE])
{
    // swap buffer
    unsigned char _l[HALF_BLOCK_SIZE];
    memcpy(_l, l, HALF_BLOCK_SIZE);

    for (int i = 0; i < HALF_BLOCK_SIZE; i++) {
        l[i] ^= r[i] ^ k[i]; // oh, I see! it's XOR-ing!
    }

    // restore
    memcpy(r, _l, HALF_BLOCK_SIZE);
}
void round_backward(const unsigned char k[HALF_BLOCK_SIZE], unsigned char l[HALF_BLOCK_SIZE], unsigned char r[HALF_BLOCK_SIZE])
{
    // swap buffer
    unsigned char _r[HALF_BLOCK_SIZE];
    memcpy(_r, r, HALF_BLOCK_SIZE);

    for (int i = 0; i < HALF_BLOCK_SIZE; i++) {
        r[i] ^= l[i] ^ k[i];
    }

    // restore
    memcpy(l, _r, HALF_BLOCK_SIZE);
}
void encrypt(const unsigned char key[KEY_LENGTH], const unsigned char pt[BLOCK_SIZE], unsigned char ct[BLOCK_SIZE])
{
    memcpy(ct, pt, BLOCK_SIZE);

    unsigned char key_part[HALF_BLOCK_SIZE];
    unsigned char* p1 = (unsigned char*)ct;
    unsigned char* p2 = (unsigned char*)(ct + HALF_BLOCK_SIZE);

    for (int r = 1; r <= ROUND_COUNT; r++) {
        memcpy(key_part, key + ((r - 1) * HALF_BLOCK_SIZE), HALF_BLOCK_SIZE);
        round_forward(key_part, p1, p2);
    }
}

void decrypt(const unsigned char key[KEY_LENGTH], const unsigned char ct[BLOCK_SIZE], unsigned char dt[BLOCK_SIZE])
{
    memcpy(dt, ct, BLOCK_SIZE);

    unsigned char key_part[HALF_BLOCK_SIZE];
    unsigned char* p1 = (unsigned char*)dt;
    unsigned char* p2 = (unsigned char*)(dt + HALF_BLOCK_SIZE);

    for (int r = ROUND_COUNT; r > 0; r--) {
        memcpy(key_part, key + ((r - 1) * HALF_BLOCK_SIZE), HALF_BLOCK_SIZE);
        round_backward(key_part, p1, p2);
    }
}


void encrypt_cbc(const unsigned char key[KEY_LENGTH], const unsigned char* iv, const unsigned char* pt, unsigned char* ct, const unsigned int pt_length)
{
    const unsigned int block_count = pt_length / BLOCK_SIZE;

    unsigned char pt_block[BLOCK_SIZE];
    unsigned char ct_block[BLOCK_SIZE];
    unsigned char* cursor_ct;

    memcpy(ct_block, iv, BLOCK_SIZE);

    int char_index = 0;

    for (int i = 0; i < block_count; i++) {
        char_index = i * BLOCK_SIZE;
        cursor_ct = (unsigned char*)(ct + char_index);

        // CBC
        for (int j = 0; j < BLOCK_SIZE; j++) {
            pt_block[j] = pt[char_index + j] ^ ct_block[j];
        }

        encrypt(key, pt_block, cursor_ct);

        memcpy(ct_block, cursor_ct, BLOCK_SIZE);
    }
}

void decrypt_cbc(const unsigned char key[KEY_LENGTH], const unsigned char* iv, const unsigned char* ct, unsigned char* dt, const unsigned int ct_length)
{
  const unsigned int block_count = ct_length / BLOCK_SIZE;

  unsigned char ct_block[BLOCK_SIZE];
  unsigned char dt_block[BLOCK_SIZE];
  unsigned char cbc_chain[BLOCK_SIZE];

  memcpy(cbc_chain, iv, BLOCK_SIZE);

  int char_index = 0;

  for (int i = 0; i < block_count; i++) {
      char_index = i * BLOCK_SIZE;

      memcpy(ct_block, (unsigned char*)(ct + char_index), BLOCK_SIZE);

      decrypt(key, ct_block, dt_block);

      // CBC
      for (int j = 0; j < BLOCK_SIZE; j++) {
          dt[char_index + j] = dt_block[j] ^ cbc_chain[j];
      }

      memcpy(cbc_chain, ct_block, BLOCK_SIZE);
  }
}

void encrypt_ecb(const unsigned char* key, const unsigned char* pt, unsigned char* ct, const unsigned int pt_length)
{
    const unsigned int block_count = pt_length / BLOCK_SIZE;

    int char_index = 0;

    for (int i = 0; i < block_count; i++) {
        char_index = i * BLOCK_SIZE;
        encrypt(key, pt + char_index, ct + char_index);
    }
}

void decrypt_ecb(const unsigned char* key, const unsigned char* ct, unsigned char* dt, const unsigned int ct_length)
{
  const unsigned int block_count = ct_length / BLOCK_SIZE;

  int char_index = 0;

  for (int i = 0; i < block_count; i++) {
      char_index = i * BLOCK_SIZE;
      decrypt(key, ct + char_index, dt + char_index);
  }
}
