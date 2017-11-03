#ifndef FIESTELXOR_MAIN_H
#define FIESTELXOR_MAIN_H

#include <string.h>
#include <stdio.h>
#include <assert.h>

#define BLOCK_SIZE      16
#define ROUND_COUNT     16
#define HALF_BLOCK_SIZE 8
#define KEY_LENGTH ROUND_COUNT * HALF_BLOCK_SIZE

void round_forward(const unsigned char k[HALF_BLOCK_SIZE], unsigned char l[HALF_BLOCK_SIZE], unsigned char r[HALF_BLOCK_SIZE]);
void round_backward(const unsigned char k[HALF_BLOCK_SIZE], unsigned char l[HALF_BLOCK_SIZE], unsigned char r[HALF_BLOCK_SIZE]);
void encrypt(const unsigned char* key, const unsigned char pt[BLOCK_SIZE], unsigned char ct[BLOCK_SIZE]);
void decrypt(const unsigned char* key, const unsigned char ct[BLOCK_SIZE], unsigned char dt[BLOCK_SIZE]);
void encrypt_cbc(const unsigned char* key, const unsigned char* iv, const unsigned char* pt, unsigned char* ct, const unsigned int pt_length);
void decrypt_cbc(const unsigned char* key, const unsigned char* iv, const unsigned char* ct, unsigned char* dt, const unsigned int ct_length);
void encrypt_ecb(const unsigned char* key, const unsigned char* pt, unsigned char* ct, const unsigned int pt_length);
void decrypt_ecb(const unsigned char* key, const unsigned char* ct, unsigned char* dt, const unsigned int ct_length);

#endif // FIESTELXOR_MAIN_H
