#include <test_main.h>
#include <main.h>

unsigned char test_key_block[KEY_LENGTH];

const unsigned char test_iv[BLOCK_SIZE] = {
    0x10, 0x11, 0x12, 0x13,
    0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b,
    0x1c, 0x1d, 0x1e, 0x1f
};

const unsigned char test_pt_block[BLOCK_SIZE] = {
    0x21, 0x22, 0x23, 0x24,
    0x25, 0x26, 0x27, 0x28,
    0x29, 0x2a, 0x2b, 0x2c,
    0x2d, 0x2e, 0x2f, 0x30,
};

const unsigned char test_pt_blocks[32] = {
    0x31, 0x32, 0x33, 0x34,
    0x35, 0x36, 0x37, 0x38,
    0x39, 0x3a, 0x3b, 0x3c,
    0x3d, 0x3e, 0x3f, 0x40,
    0x41, 0x42, 0x43, 0x44,
    0x45, 0x46, 0x47, 0x48,
    0x49, 0x4a, 0x4b, 0x4c,
    0x4d, 0x4e, 0x4f, 0x50,
};

const unsigned char test_round_l[HALF_BLOCK_SIZE] = { 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80 };
const unsigned char test_round_r[HALF_BLOCK_SIZE] = { 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7 };

void generate_key()
{
    for (unsigned char c = 0x00; c < KEY_LENGTH; c++) {
        test_key_block[c] = c;
    }
}

TEST_F(FeilstelXorTest, RoundForward)
{
    generate_key();

    unsigned char key_part[HALF_BLOCK_SIZE];
    unsigned char pt1[HALF_BLOCK_SIZE];
    unsigned char pt2[HALF_BLOCK_SIZE];

    bool encrypted = false;

    memcpy(key_part, test_key_block, HALF_BLOCK_SIZE);
    memcpy(pt1, test_round_l, HALF_BLOCK_SIZE);
    memcpy(pt2, test_round_r, HALF_BLOCK_SIZE);

    ROUND_FORWARD(key_part, pt1, pt2);
    for (int j = 0; j < HALF_BLOCK_SIZE; j++) {
        if (pt1[j] != test_round_l[j]) {
            encrypted = true;
            break;
        }
    }

    ASSERT_TRUE(encrypted);
}

TEST_F(FeilstelXorTest, RoundBackward)
{
    generate_key();

    unsigned char key_part[HALF_BLOCK_SIZE];
    unsigned char pt1[HALF_BLOCK_SIZE];
    unsigned char pt2[HALF_BLOCK_SIZE];

    memcpy(pt1, test_round_l, HALF_BLOCK_SIZE);
    memcpy(pt2, test_round_r, HALF_BLOCK_SIZE);

    memcpy(key_part, test_key_block, HALF_BLOCK_SIZE);
    ROUND_FORWARD(key_part, pt1, pt2);
    memcpy(key_part, test_key_block + HALF_BLOCK_SIZE, HALF_BLOCK_SIZE);
    ROUND_FORWARD(key_part, pt1, pt2);

    ROUND_BACKWORD(key_part, pt1, pt2);
    memcpy(key_part, test_key_block, HALF_BLOCK_SIZE);
    ROUND_BACKWORD(key_part, pt1, pt2);

    for (int i = 0; i < HALF_BLOCK_SIZE; i++) {
        ASSERT_EQ(pt1[i], test_round_l[i]);
        ASSERT_EQ(pt2[i], test_round_r[i]);
    }

    memcpy(pt1, test_round_l, HALF_BLOCK_SIZE);
    memcpy(pt2, test_round_r, HALF_BLOCK_SIZE);

    for (int i = 1; i <= ROUND_COUNT; i++) {
        memcpy(key_part, test_key_block + ((i - 1) * HALF_BLOCK_SIZE), HALF_BLOCK_SIZE);
        ROUND_FORWARD(key_part, pt1, pt2);
    }
    for (int i = ROUND_COUNT; i > 0; i--) {
        memcpy(key_part, test_key_block + ((i - 1) * HALF_BLOCK_SIZE), HALF_BLOCK_SIZE);
        ROUND_BACKWORD(key_part, pt1, pt2);
    }

    for (int i = 0; i < HALF_BLOCK_SIZE; i++) {
        ASSERT_EQ(pt1[i], test_round_l[i]);
        ASSERT_EQ(pt2[i], test_round_r[i]);
    }
}

TEST_F(FeilstelXorTest, Encrypt)
{
    generate_key();

    unsigned char ct_block[BLOCK_SIZE];
    unsigned char dt_block[BLOCK_SIZE];

    ENCRYPT(test_key_block, test_pt_block, ct_block);

    bool encrypted = false;
    for (int i = 0; i < BLOCK_SIZE; i++) {
        if (ct_block[i] != test_pt_block[i]) {
          encrypted = true;
          break;
        }
    }

    ASSERT_TRUE(encrypted);
}

TEST_F(FeilstelXorTest, Decrypt)
{
    generate_key();

    unsigned char ct_block[BLOCK_SIZE];
    unsigned char dt_block[BLOCK_SIZE];

    ENCRYPT(test_key_block, test_pt_block, ct_block);
    DECRYPT(test_key_block, ct_block, dt_block);

    for (int i = 0; i < BLOCK_SIZE; i++)
        ASSERT_EQ(test_pt_block[i], dt_block[i]);
}


TEST_F(FeilstelXorTest, EncryptCBC)
{
    generate_key();

    unsigned char ct_blocks[32];

    ENCRYPT_CBC(test_key_block, test_iv, test_pt_blocks, ct_blocks, 32);

    bool encrypted = false;
    for (int i = 0; i < 32; i++) {
        if (ct_blocks[i] != test_pt_blocks[i]) {
          encrypted = true;
          break;
        }
    }

    ASSERT_TRUE(encrypted);
}

TEST_F(FeilstelXorTest, DecryptCBC)
{
    generate_key();

    unsigned char ct_blocks[32];
    unsigned char dt_blocks[32];

    ENCRYPT_CBC(test_key_block, test_iv, test_pt_blocks, ct_blocks, 32);
    DECRYPT_CBC(test_key_block, test_iv, ct_blocks, dt_blocks, 32);

    for (int i = 0; i < 32; i++) {
        ASSERT_EQ(dt_blocks[i], test_pt_blocks[i]);
    }
}

TEST_F(FeilstelXorTest, EncryptECB)
{
    generate_key();

    unsigned char ct_blocks[32];

    ENCRYPT_ECB(test_key_block, test_pt_blocks, ct_blocks, 32);

    bool encrypted = false;
    for (int i = 0; i < 32; i++) {
        if (ct_blocks[i] != test_pt_blocks[i]) {
          encrypted = true;
          break;
        }
    }

    ASSERT_TRUE(encrypted);
}

TEST_F(FeilstelXorTest, DecryptECB)
{
    generate_key();

    unsigned char ct_blocks[32];
    unsigned char dt_blocks[32];

    ENCRYPT_ECB(test_key_block, test_pt_blocks, ct_blocks, 32);
    DECRYPT_ECB(test_key_block, ct_blocks, dt_blocks, 32);

    for (int i = 0; i < 32; i++) {
        ASSERT_EQ(dt_blocks[i], test_pt_blocks[i]);
    }
}
