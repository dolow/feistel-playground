#ifndef FIESTELXOR_MAIN_H
#define FIESTELXOR_MAIN_H

#include <string.h>
#include <stdio.h>
#include <assert.h>

#define BLOCK_SIZE      16
#define ROUND_COUNT     16
#define HALF_BLOCK_SIZE 8
#define KEY_LENGTH ROUND_COUNT * HALF_BLOCK_SIZE

#define ROUND_FORWARD  transform_rgba
#define ROUND_BACKWORD restore_rgba
#define ENCRYPT        compress
#define DECRYPT        decompress
#define ENCRYPT_CBC    bmp_to_pvrtc
#define DECRYPT_CBC    pvrtc_to_bmp
#define ENCRYPT_ECB    bmp_to_etc2
#define DECRYPT_ECB    etc2_to_bmp

void ROUND_FORWARD(const unsigned char k[HALF_BLOCK_SIZE], unsigned char l[HALF_BLOCK_SIZE], unsigned char r[HALF_BLOCK_SIZE]);
void ROUND_BACKWORD(const unsigned char k[HALF_BLOCK_SIZE], unsigned char l[HALF_BLOCK_SIZE], unsigned char r[HALF_BLOCK_SIZE]);
void ENCRYPT(const unsigned char* key, const unsigned char pt[BLOCK_SIZE], unsigned char ct[BLOCK_SIZE]);
void DECRYPT(const unsigned char* key, const unsigned char ct[BLOCK_SIZE], unsigned char dt[BLOCK_SIZE]);
void ENCRYPT_CBC(const unsigned char* key, const unsigned char* iv, const unsigned char* pt, unsigned char* ct, const unsigned int pt_length);
void DECRYPT_CBC(const unsigned char* key, const unsigned char* iv, const unsigned char* ct, unsigned char* dt, const unsigned int ct_length);
void ENCRYPT_ECB(const unsigned char* key, const unsigned char* pt, unsigned char* ct, const unsigned int pt_length);
void DECRYPT_ECB(const unsigned char* key, const unsigned char* ct, unsigned char* dt, const unsigned int ct_length);

#endif // FIESTELXOR_MAIN_H
