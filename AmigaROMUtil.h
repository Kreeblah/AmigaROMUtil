/*
MIT License

Copyright (c) 2021 Christopher Gelatt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _AMIGAROMUTIL_H
#define _AMIGAROMUTIL_H

#define MAX_AMIGA_ROM_SIZE (1024 * 512) + 11

#include "AmigaROMHashes.h"

#include <stdbool.h>
#include <stdint.h>

// Returns the size of the ROM, with 0 for failure
// For safety, rom_contents should be the size of
// the largest Amiga ROM plus 11 bytes, to allow for encryption
uint32_t ReadAmigaROM(const char *filename, uint8_t *rom_contents);

// Detects the version of the ROM by SHA1 hash
// Returns NULL for failure, else a string indicating the ROM version
const char* DetectAmigaROMVersion(const uint8_t *rom_contents, const size_t rom_size);

// Returns a character indicating the type of ROM detected based on the SHA1 hash
// Valid return values are:
// A - Kickstart A ROM
// B - Kickstart B ROM
// M - Kickstart merged ROM
// O - Other (non-Kickstart) ROM
// U - Unknown ROM
char DetectAmigaROMType(const uint8_t *rom_contents, const size_t rom_size);

// Returns an unsigned 32-bit integer with the checksum for the ROM.
// This checksum is used by Amigas to validate whether a ROM is undamaged,
// and is located 24 bytes from the end of the ROM.
// If calc_new_sum is true, then the function will return a checksum value which
// will be accepted for use by an Amiga system.
uint32_t CalculateAmigaROMChecksum(const uint8_t *rom_contents, const size_t rom_size, const bool calc_new_sum);

// Returns the checksum embedded in the ROM, or 0 if it fails.
// Technically a ROM could have a valid checksum of zero, but 
// this is exceedingly unlikely.
uint32_t GetEmbeddedAmigaROMChecksum(const uint8_t *rom_contents, const size_t rom_size);

// Returns a boolean indicating whether the calculated checksum in the ROM
// matches the calculated checksum for the ROM.
bool ValidateAmigaROMChecksum(const uint8_t *rom_contents, const size_t rom_size);

// Calculates and embeds a correct checksum in the ROM.
// Returns true if it succeeds, or false if it fails.
bool CorrectAmigaROMChecksum(uint8_t *rom_contents, const size_t rom_size);

// Detects whether an Amiga ROM is encrypted.
// Returns true if it is, or false if it isn't.
bool DetectAmigaROMEncryption(const uint8_t *rom_contents);

// Encrypts or decrypts Amiga ROMs according to whether crypt_operation is true or false.
// If true, it will encrypt the ROMs.  If false, it will decrypt them.
// The function returns the new size of the ROM.  If the operation failed,
// it will be the same size as the original ROM was.
// For safety, make sure that rom_contents has space for 11 more bytes than the original
// ROM had if an encrypt operation is performed.
size_t CryptAmigaROM(uint8_t *rom_contents, const size_t rom_size, bool crypt_operation, const char *keyfile_path);

// 0 indicates the ROM is not byte swapped (the ROM is for emulators)
// 1 indicates the ROM is byte swapped (the ROM is for physical ICs)
// -1 indicates the ROM is not an Amiga ROM known to this library
int DetectAmigaROMByteSwap(const uint8_t *rom_contents, const size_t rom_size);

// If swap_unconditionally is true, the method will swap the ROM's bytes regardless of whether
// or not it is a known ROM.
// Returns 1 for success or 0 for failure.
int SetAmigaROMByteSwap(uint8_t *rom_contents, const size_t rom_size, bool swap_bytes, bool unswap_bytes, bool swap_unconditionally);

// For this method, ROM A and ROM B should each be the same size as the merged ROM.
// Each A and B ROM gets the same contents repeated twice.
void SplitAmigaROM(const uint8_t *merged_rom_contents, const size_t merged_rom_size, uint8_t *rom_high_contents, uint8_t *rom_low_contents);

// For this method, ROM A and ROM B should each be the same size as the merged ROM.
// Each A and B ROM gets the same contents repeated twice.
void MergeAmigaROM(const uint8_t *rom_high_contents, const uint8_t *rom_low_contents, const size_t split_rom_size, uint8_t *merged_rom_contents);

#endif