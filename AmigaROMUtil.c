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

#include "AmigaROMUtil.h"
#include "AmigaROMHashes.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__APPLE__) && defined(__MACH__)

#include <libkern/OSByteOrder.h>

#define htobe32(x) OSSwapHostToBigInt32(x)
#define be32toh(x) OSSwapBigToHostInt32(x)

#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__DragonFly__)

#include <sys/endian.h>

#elif defined(__OpenBSD__)

#include <sys/endian.h>

#define be32toh(x) betoh32(x)

#elif defined(__linux__) || defined(__CYGWIN__)

#include <endian.h>

#elif defined(_WIN32) || defined(_WIN64)

#if !defined(_M_ARM) && !defined(_M_ARM64)

#if defined(_MSC_VER)

#include <stdlib.h>

#define htobe32(x) _byteswap_ulong(x)
#define be32toh(x) _byteswap_ulong(x)

#elif defined(__GNUC__) || defined(__clang__) // Compiler (Windows, Intel)

#define htobe32(x) __builtin_bswap32(x)
#define be32toh(x) __builtin_bswap32(x)

#endif // Compiler (Windows, Intel)

#else // Architecture (Windows, ARM)

#define htobe32(x) (x)
#define be32toh(x) (x)

#endif // Architecture (Windows)

#endif // Platform

extern int sha1digest(uint8_t *digest, char *hexdigest, const uint8_t *data, size_t databytes);

// Returns the size of the ROM, with 0 for failure
// For safety, rom_contents should be the size of
// the largest Amiga ROM plus 11 bytes, to allow for encryption
uint32_t ReadAmigaROM(const char *filename, uint8_t *rom_contents)
{
	uint8_t *buffer;
	FILE *fp;

	size_t file_size;

	buffer = (uint8_t*)malloc(MAX_AMIGA_ROM_SIZE * sizeof(uint8_t));

	if(!buffer)
	{
		return 0;
	}

	memset(buffer, 0, MAX_AMIGA_ROM_SIZE);

	fp = fopen(filename, "rb");
	if(!fp)
	{
		free(buffer);
		return 0;
	}

	file_size = fread(buffer, sizeof(uint8_t), MAX_AMIGA_ROM_SIZE, fp);

	fclose(fp);

	// Check to see whether file_size is a power of two
	if((file_size != 0) && ((file_size & (file_size - 1)) == 0))
	{
		memcpy(rom_contents, buffer, file_size);
		free(buffer);
		return file_size;
	}
	else if((file_size != 0) && (((file_size - 11) & (file_size - 12)) == 0) && DetectAmigaROMEncryption(buffer)) // Is it encrypted?
	{
		memcpy(rom_contents, buffer, file_size);
		free(buffer);
		return file_size;
	}

	free(buffer);
	return 0;	
}

// Detects the version of the ROM by SHA1 hash
// Returns NULL for failure, else a string indicating the ROM version
const char* DetectAmigaROMVersion(const uint8_t *rom_contents, const size_t rom_size)
{
	uint8_t *digest;
	char *hexdigest;

	size_t i;
	size_t rom_quantity = sizeof(AMIGA_ROM_INFO) / sizeof(struct AmigaROMInfo);

	digest = (uint8_t*)malloc(20 * sizeof(uint8_t));
	if(!digest)
	{
		return NULL;
	}

	hexdigest = (char*)malloc(41 * sizeof(char));
	if(!hexdigest)
	{
		free(digest);
		return NULL;
	}

	if (sha1digest(digest, hexdigest, rom_contents, rom_size))
	{
		free(digest);
		free(hexdigest);
		return NULL;
	}

	for (i = 0; i < rom_quantity; i++)
	{
		if (strncmp(hexdigest, AMIGA_ROM_INFO[i].sha1hash, 40) == 0)
		{
			free(digest);
			free(hexdigest);
			return AMIGA_ROM_INFO[i].version;
		}
	}

	free(digest);
	free(hexdigest);
	return NULL;
}

// Returns a character indicating the type of ROM detected based on the SHA1 hash
// Valid return values are:
// A - Kickstart A ROM
// B - Kickstart B ROM
// M - Kickstart merged ROM
// O - Other (non-Kickstart) ROM
// U - Unknown ROM
char DetectAmigaROMType(const uint8_t *rom_contents, const size_t rom_size)
{
	uint8_t *digest;
	char *hexdigest;

	size_t i;
	size_t rom_quantity = sizeof(AMIGA_ROM_INFO) / sizeof(struct AmigaROMInfo);

	digest = (uint8_t*)malloc(20 * sizeof(uint8_t));
	if(!digest)
	{
		return 'U';
	}

	hexdigest = (char*)malloc(41 * sizeof(char));
	if(!hexdigest)
	{
		free(digest);
		return 'U';
	}

	if (sha1digest(digest, hexdigest, rom_contents, rom_size))
	{
		free(digest);
		free(hexdigest);
		return 'U';
	}

	for (i = 0; i < rom_quantity; i++)
	{
		if (strncmp(hexdigest, AMIGA_ROM_INFO[i].sha1hash, 40) == 0)
		{
			free(digest);
			free(hexdigest);
			return AMIGA_ROM_INFO[i].type;
		}
	}

	free(digest);
	free(hexdigest);
	return 'U';
}

// Returns an unsigned 32-bit integer with the checksum for the ROM.
// This checksum is used by Amigas to validate whether a ROM is undamaged,
// and is located 24 bytes from the end of the ROM.
// If calc_new_sum is true, then the function will return a checksum value which
// will be accepted for use by an Amiga system.
uint32_t CalculateAmigaROMChecksum(const uint8_t *rom_contents, const size_t rom_size, const bool calc_new_sum)
{
	uint32_t *rom_contents_32 = (uint32_t*)rom_contents;

	uint32_t sum = 0;
	uint32_t temp = 0;
	size_t i = 0;

	if(rom_size < 24)
	{
		return 0xFFFFFFFF;
	}

	for(i = 0; i < rom_size / 4; i++)
	{
		if(calc_new_sum)
		{
			if(i != (rom_size - 24) / 4)
			{
				temp = be32toh(rom_contents_32[i]);
			}
			else
			{
				temp = 0;
			}
		}
		else
		{
			temp = be32toh(rom_contents_32[i]);
		}

		if(sum + temp < sum)
		{
			sum++;
		}

		sum += temp;
	}

	return ~sum;
}

// Returns the checksum embedded in the ROM, or 0 if it fails.
// Technically a ROM could have a valid checksum of zero, but 
// this is exceedingly unlikely.
uint32_t GetEmbeddedAmigaROMChecksum(const uint8_t *rom_contents, const size_t rom_size)
{
	uint32_t *rom_contents_32 = (uint32_t*)rom_contents;

	if(rom_size < 24)
	{
		return 0;
	}

	return be32toh(rom_contents_32[(rom_size - 24) / 4]);
}

// Returns a boolean indicating whether the calculated checksum in the ROM
// matches the calculated checksum for the ROM.
bool ValidateAmigaROMChecksum(const uint8_t *rom_contents, const size_t rom_size)
{
	uint32_t calculated_sum = CalculateAmigaROMChecksum(rom_contents, rom_size, false);

	return (calculated_sum == 0);
}

// Calculates and embeds a correct checksum in the ROM.
// Returns true if it succeeds, or false if it fails.
bool CorrectAmigaROMChecksum(uint8_t *rom_contents, const size_t rom_size)
{
	uint32_t *rom_contents_32;
	uint8_t *temp_rom_contents;

	uint32_t old_sum = 0;
	uint32_t new_sum = 0;

	temp_rom_contents = (uint8_t*)malloc(rom_size * sizeof(uint8_t));
	if(!temp_rom_contents)
	{
		return false;
	}

	memcpy(temp_rom_contents, rom_contents, rom_size);

	rom_contents_32 = (uint32_t*)temp_rom_contents;
	old_sum = be32toh(rom_contents[(rom_size - 24) / 4]);
	new_sum = CalculateAmigaROMChecksum(temp_rom_contents, rom_size, true);

	if(old_sum == new_sum)
	{
		free(temp_rom_contents);
		return true;
	}

	if(new_sum == 0)
	{
		free(temp_rom_contents);
		return false;
	}

	rom_contents_32[(rom_size - 24) / 4] = htobe32(new_sum);

	if(!ValidateAmigaROMChecksum(rom_contents, rom_size))
	{
		free(temp_rom_contents);
		return false;
	}

	memcpy(rom_contents, temp_rom_contents, rom_size);

	free(temp_rom_contents);
	return true;
}

// Detects whether an Amiga ROM is encrypted.
// Returns true if it is, or false if it isn't.
bool DetectAmigaROMEncryption(const uint8_t *rom_contents)
{
	return (strncmp((char*)rom_contents, "AMIROMTYPE1", 11) == 0);
}

// Encrypts or decrypts Amiga ROMs according to whether crypt_operation is true or false.
// If true, it will encrypt the ROMs.  If false, it will decrypt them.
// The function returns the new size of the ROM.  If the operation failed,
// it will be the same size as the original ROM was.
// For safety, make sure that rom_contents has space for 11 more bytes than the original
// ROM had if an encrypt operation is performed.
size_t CryptAmigaROM(uint8_t *rom_contents, const size_t rom_size, bool crypt_operation, const char *keyfile_path)
{
	FILE *fp;
	uint8_t *keyfile_buffer;
	uint8_t *result_buffer;

	bool is_encrypted = false;
	size_t result_size = rom_size;
	size_t keyfile_size = 0;
	size_t i = 0;
	size_t key_idx = 0;

	is_encrypted = DetectAmigaROMEncryption(rom_contents);
	if(is_encrypted == crypt_operation)
	{
		return rom_size;
	}

	keyfile_buffer = (uint8_t*)malloc(MAX_AMIGA_ROM_SIZE * sizeof(uint8_t));
	if(!keyfile_buffer)
	{
		return rom_size;
	}

	if(crypt_operation)
	{
		result_buffer = (uint8_t*)malloc((rom_size + 11) * sizeof(uint8_t));
	}
	else
	{
		result_buffer = (uint8_t*)malloc((rom_size - 11) * sizeof(uint8_t));
	}
	if(!result_buffer)
	{
		free(keyfile_buffer);
		return rom_size;
	}

	memset(result_buffer, 0, MAX_AMIGA_ROM_SIZE);
	memset(keyfile_buffer, 0, MAX_AMIGA_ROM_SIZE);

	fp = fopen(keyfile_path, "rb");
	if(!fp)
	{
		free(keyfile_buffer);
		free(result_buffer);
		return rom_size;
	}

	keyfile_size = fread(keyfile_buffer, sizeof(uint8_t), MAX_AMIGA_ROM_SIZE, fp);

	fclose(fp);

	if(keyfile_size < 1)
	{
		free(keyfile_buffer);
		free(result_buffer);
		return rom_size;
	}

	if(is_encrypted)
	{
		result_size = result_size - 11;
		memcpy(result_buffer, &rom_contents[11], result_size);
	}
	else
	{
		memcpy(result_buffer, rom_contents, result_size);
	}

	for(i = 0; i < result_size; i++)
	{
		result_buffer[i] ^= keyfile_buffer[key_idx];

		key_idx = (key_idx + 1) % keyfile_size;
	}

	if(is_encrypted)
	{
		memcpy(rom_contents, result_buffer, result_size);
	}
	else
	{
		snprintf((char*)rom_contents, 12, "AMIROMTYPE1");
		memcpy(&rom_contents[11], result_buffer, result_size);
		result_size = result_size + 11;
	}

	free(keyfile_buffer);
	free(result_buffer);
	return result_size;
}

// 0 indicates the ROM is not byte swapped (the ROM is for emulators)
// 1 indicates the ROM is byte swapped (the ROM is for physical ICs)
// -1 indicates the ROM is not an Amiga ROM known to this library
int DetectAmigaROMByteSwap(const uint8_t *rom_contents, const size_t rom_size)
{
	uint8_t *digest;
	char *hexdigest;

	size_t i;
	size_t rom_quantity = sizeof(AMIGA_ROM_INFO) / sizeof(struct AmigaROMInfo);

	digest = (uint8_t*)malloc(20 * sizeof(uint8_t));
	if(!digest)
	{
		return -1;
	}

	hexdigest = (char*)malloc(41 * sizeof(char));
	if(!hexdigest)
	{
		free(digest);
		return -1;
	}

	if (sha1digest(digest, hexdigest, rom_contents, rom_size))
	{
		free(digest);
		free(hexdigest);
		return -1;
	}

	for (i = 0; i < rom_quantity; i++)
	{
		if (strncmp(hexdigest, AMIGA_ROM_INFO[i].sha1hash, 40) == 0)
		{
			free(digest);
			free(hexdigest);
			return AMIGA_ROM_INFO[i].byte_swap;
		}
	}

	free(digest);
	free(hexdigest);
	return -1;
}

// If swap_unconditionally is true, the method will swap the ROM's bytes regardless of whether
// or not it is a known ROM.
// Returns 1 for success or 0 for failure.
int SetAmigaROMByteSwap(uint8_t *rom_contents, const size_t rom_size, bool swap_bytes, bool unswap_bytes, bool swap_unconditionally)
{
	size_t i;
	int is_swapped;

	if(swap_bytes && unswap_bytes && !swap_unconditionally)
	{
		return 0;
	}

	if(!swap_unconditionally)
	{
		is_swapped = DetectAmigaROMByteSwap(rom_contents, rom_size);
		if (is_swapped < 0)
		{
			return 0;
		}
	}

	if (swap_unconditionally || (is_swapped == 0 && swap_bytes) || (is_swapped == 1 && unswap_bytes))
	{
		for (i = 0; i < rom_size; i += 2)
		{
			// In-place XOR swap of two bytes
			rom_contents[i] ^= rom_contents[i + 1];
			rom_contents[i + 1] ^= rom_contents[i];
			rom_contents[i] ^= rom_contents[i + 1];
		}
	}

	return 1;
}

// For this method, ROM A and ROM B should each be the same size as the merged ROM.
// Each A and B ROM gets the same contents repeated twice.
void SplitAmigaROM(const uint8_t *merged_rom_contents, const size_t merged_rom_size, uint8_t *rom_high_contents, uint8_t *rom_low_contents)
{
	size_t i;

	for(i = 0; i < merged_rom_size; i = i + 4)
	{
		memcpy(&rom_high_contents[i / 2], &merged_rom_contents[i], 2);
		memcpy(&rom_low_contents[i / 2], &merged_rom_contents[i + 2], 2);
	}

	memcpy(&rom_high_contents[merged_rom_size / 2], &rom_high_contents[0], merged_rom_size / 2);
	memcpy(&rom_low_contents[merged_rom_size / 2], &rom_low_contents[0], merged_rom_size / 2);
}

// For this method, ROM A and ROM B should each be the same size as the merged ROM.
// Each A and B ROM gets the same contents repeated twice.
void MergeAmigaROM(const uint8_t *rom_high_contents, const uint8_t *rom_low_contents, const size_t split_rom_size, uint8_t *merged_rom_contents)
{
	size_t i;

	for(i = 0; i < split_rom_size / 2; i++)
	{
		memcpy(&merged_rom_contents[i * 4], &rom_high_contents[i * 2], 2);
		memcpy(&merged_rom_contents[(i * 4) + 2], &rom_low_contents[i * 2], 2);
	}
}
