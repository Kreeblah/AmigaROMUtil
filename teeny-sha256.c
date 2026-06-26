/*******************************************************************************
 * Teeny SHA-1
 *
 * The below SHA256() calculates a SHA-1 hash value for a
 * specified data buffer and generates a hex representation of the
 * result.  Modifications from the upstream source are to contain the file to a
 * single C source file, generate an ASCII hex digest, and compile under strict
 * C17 standards.
 * 
 * Original source repo: https://github.com/CurryB0i/SHA-256-in-C
 *
 * Copyright (c) 2025 CurryB0i
 *
 * License: MIT, see included LICENSE file for details.
 *
 * To use the SHA256() function either copy it into an existing
 * project source code file or include this file in a project and put
 * the declaration (example below) in the sources files where needed.
 ******************************************************************************/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Declaration:
extern int SHA256(const uint8_t* msg, size_t msgLen, uint8_t* digest);
*/

/**
 * Computes the SHA-256 hash of the given input message.
 *
 * @param msg        Pointer to the input message (raw bytes).
 * @param msgLen     Length of the input message in bytes.
 * @param digest     Pointer to a buffer of at least SHA256_DIGEST_SIZE bytes.
 *                   The resulting 32-byte hash will be written here.
 * @param hexDigest  An ASCII hex string representation of the SHA256 digest.
 *
 * @return           0 on success, non-zero on failure.
 */
int SHA256(const uint8_t* msg, size_t msgLen, uint8_t* digest, char* hexDigest);

void padMsg(uint8_t* paddedBinaryMsg,const uint8_t* binary,int len,int padLen) {
  memcpy(paddedBinaryMsg,binary,len*sizeof(uint8_t));
  paddedBinaryMsg[len++] = 0x80;
  while(padLen > 0) {
    paddedBinaryMsg[len++] = 0;
    padLen--;
  }
}

void intToBin(uint8_t* paddedBinaryMsg, int offset, uint64_t len) {
  for (size_t i = 0; i < 8; i++) {
    paddedBinaryMsg[offset + i] = (len >> (56 - 8 * i)) & 0xFF;
  }
}

int getBlocks(uint8_t** blocks, int noOfBlocks, uint8_t* binMsg) {
  for(int i = 0;i < noOfBlocks; i++) {
    blocks[i] = malloc(64 * sizeof(uint8_t));
    if(blocks[i] == NULL) {
      fprintf(stderr, "Memory allocation failed\n");
      free(binMsg);
      return 1;
    }

    memcpy(blocks[i],binMsg + 64*i, 64*sizeof(uint8_t));
  }
  return 0;
}

uint32_t rightRotate(const uint32_t word,int offset) {
  uint32_t temp = word;
  offset = offset % 32;
  return (temp >> offset) | (temp << (32 - offset));
}

void hash (uint8_t** blocks,int noOfBlocks,uint8_t* hashedMsg) {

  uint32_t H[] = {
    0x6a09e667,
    0xbb67ae85,
    0x3c6ef372,
    0xa54ff53a,
    0x510e527f,
    0x9b05688c,
    0x1f83d9ab,
    0x5be0cd19
  };

  static uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
  };

  for(int i = 0;i < noOfBlocks; i++) {
    uint32_t W[64];

    for(size_t j=0; j<16; j++) {
      W[j] = (((uint32_t)blocks[i][j * 4] << 24) |
          ((uint32_t)blocks[i][j * 4 + 1] << 16) |
          ((uint32_t)blocks[i][j * 4 + 2] << 8) |
          ((uint32_t)blocks[i][j * 4 + 3]));
    }

    for(size_t j=16; j<64; j++) {
      uint32_t s0 = rightRotate(W[j-15],7) ^ rightRotate(W[j-15],18) ^ (W[j-15] >> 3);
      uint32_t s1 = rightRotate(W[j-2],17) ^ rightRotate(W[j-2],19) ^ (W[j-2] >> 10);
      W[j] = W[j-16] + s0 + W[j-7] + s1;
    }

    uint32_t a = H[0];
    uint32_t b = H[1];
    uint32_t c = H[2];
    uint32_t d = H[3];
    uint32_t e = H[4];
    uint32_t f = H[5];
    uint32_t g = H[6];
    uint32_t h = H[7];

    uint32_t S1,ch,temp1,S0,maj,temp2;
    for(size_t j=0; j<64; j++) {
      S1 = rightRotate(e , 6) ^ rightRotate(e , 11) ^ rightRotate(e , 25);
      ch = (e & f) ^ (~e & g);
      temp1 = h + S1 + ch + K[j] + W[j];
      S0 = rightRotate(a , 2) ^ rightRotate(a , 13) ^ rightRotate(a , 22);
      maj = (a & b) ^ (a & c) ^ (b & c);
      temp2 = S0 + maj;

      h = g;
      g = f;
      f = e;
      e = d + temp1;
      d = c;
      c = b;
      b = a;
      a = temp1 + temp2;
    }

    H[0] = H[0] + a;
    H[1] = H[1] + b;
    H[2] = H[2] + c;
    H[3] = H[3] + d;
    H[4] = H[4] + e;
    H[5] = H[5] + f;
    H[6] = H[6] + g;
    H[7] = H[7] + h;
  }

  for(int i=0; i<8; i++) {
    hashedMsg[i*4 + 0] = (H[i] >> 24) & 0xFF;
    hashedMsg[i*4 + 1] = (H[i] >> 16) & 0xFF;
    hashedMsg[i*4 + 2] = (H[i] >> 8) & 0xFF;
    hashedMsg[i*4 + 3] = (H[i] >> 0) & 0xFF;
  }
}

int SHA256(const uint8_t* msg, size_t msgLen, uint8_t* digest, char* hexDigest) {

  uint8_t* binary = malloc(msgLen * sizeof(uint8_t));
  if (binary == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    return 1;
  }
  memcpy(binary, msg, msgLen);

  int padLen = 0;
  if (msgLen % 64 < 56){
    padLen = 56 - (msgLen % 64);
  } else {
    padLen = 64 + 56 - (msgLen % 64);
  }

  int paddedMsgLen = msgLen + padLen + 8;
  uint8_t* paddedBinaryMsg = malloc(paddedMsgLen * sizeof(uint8_t));
  if(paddedBinaryMsg == NULL) {
    fprintf(stderr,"Memory allocation failed");
    free(binary);
    return 1;
  }

  padMsg(paddedBinaryMsg, binary, msgLen, padLen);
  intToBin(paddedBinaryMsg, msgLen+padLen, (uint64_t) msgLen*8);
  free(binary);

  int noOfBlocks = paddedMsgLen/64;
  uint8_t** blocks = malloc(noOfBlocks * sizeof(uint8_t*));
  if(blocks == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    free(paddedBinaryMsg);
    return 1;
  }
  int status = getBlocks(blocks, noOfBlocks, paddedBinaryMsg);
  if(status != 0) {
    fprintf(stderr, "Failed to create blocks.");
    free(paddedBinaryMsg);
    return 1;
  }
  free(paddedBinaryMsg);

  uint8_t* hashedMsg = malloc(64 * sizeof(uint8_t));
  hash(blocks, noOfBlocks, hashedMsg);
  memcpy(digest, hashedMsg, 32);

  if (hexDigest) {
    for(int i=0; i<32; i++) {
      sprintf(hexDigest + 2*i, "%02x", (unsigned int)digest[i]);
    }
  }

  for(int i=0; i<noOfBlocks; i++) {
    free(blocks[i]);
  }
  free(blocks);
  free(hashedMsg);
  return 0;
}
