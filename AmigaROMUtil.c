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

#define htobe16(x) OSSwapHostToBigInt16(x)
#define be16toh(x) OSSwapBigToHostInt16(x)

#define htobe32(x) OSSwapHostToBigInt32(x)
#define be32toh(x) OSSwapBigToHostInt32(x)

#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__DragonFly__)

#include <sys/endian.h>

#elif defined(__OpenBSD__)

#include <sys/endian.h>

#define be16toh(x) betoh16(x)
#define be32toh(x) betoh32(x)

#elif defined(__linux__) || defined(__CYGWIN__)

#include <endian.h>

#elif defined(_WIN32) || defined(_WIN64)

#if !defined(_M_ARM) && !defined(_M_ARM64)

#if defined(_MSC_VER)

#include <stdlib.h>

#define htobe16(x) _byteswap_ushort(x)
#define be16toh(x) _byteswap_ushort(x)

#define htobe32(x) _byteswap_ulong(x)
#define be32toh(x) _byteswap_ulong(x)

#elif defined(__GNUC__) || defined(__clang__) // Compiler (Windows, Intel)

#define htobe16(x) __builtin_bswap16(x)
#define be16toh(x) __builtin_bswap16(x)

#define htobe32(x) __builtin_bswap32(x)
#define be32toh(x) __builtin_bswap32(x)

#endif // Compiler (Windows, Intel)

#else // Architecture (Windows, ARM)

#define htobe16(x) (x)
#define be16toh(x) (x)

#define htobe32(x) (x)
#define be32toh(x) (x)

#endif // Architecture (Windows)

#endif // Platform

#define AMIGA_256_ROM_HEADER 0x11114EF9
#define AMIGA_512_ROM_HEADER 0x11144EF9
#define AMIGA_EXT_ROM_HEADER 0x11144EF9
#define AMIGA_512_REKICK_ROM_HEADER 0x11164EF9 //TODO: Properly detect size/handle these

extern int sha1digest(uint8_t *digest, char *hexdigest, const uint8_t *data, size_t databytes);

// Returns a parsed ROM data struct, with the ROM data and size included.
// If anything files, parsed_rom will be false.  If encrypted, the ROM
// will be decrypted.
ParsedAmigaROMData ReadAmigaROM(const char *rom_file_path, const char *keyfile_path)
{
	FILE *fp;
	uint8_t *rom_data;
	uint8_t *rom_sized_data;

	ParsedAmigaROMData parsed_rom_data = {};
	size_t rom_size;

	parsed_rom_data.parsed_rom = false;
	parsed_rom_data.rom_data = NULL;
	parsed_rom_data.rom_size = 0;
	parsed_rom_data.is_encrypted = false;
	parsed_rom_data.can_decrypt = false;
	parsed_rom_data.successfully_decrypted = false;
	parsed_rom_data.header = 'U';
	parsed_rom_data.type = 'U';
	parsed_rom_data.version = NULL;
	parsed_rom_data.is_kickety_split = false;
	parsed_rom_data.valid_footer = false;

	rom_data = (uint8_t*)malloc(MAX_AMIGA_ROM_SIZE * sizeof(uint8_t));

	if(!rom_data)
	{
		return parsed_rom_data;
	}

	memset(rom_data, 0, MAX_AMIGA_ROM_SIZE);

	fp = fopen(rom_file_path, "rb");
	if(!fp)
	{
		free(rom_data);
		return parsed_rom_data;
	}

	rom_size = fread(rom_data, sizeof(uint8_t), MAX_AMIGA_ROM_SIZE, fp);

	fclose(fp);

	rom_sized_data = (uint8_t*)malloc(rom_size * sizeof(uint8_t));
	memcpy(rom_sized_data, rom_data, rom_size);
	free(rom_data);
	rom_data = rom_sized_data;

	// Check to see whether parsed_rom_data.rom_size is large enough and a power of two
	if(!ValidateAmigaROMSize(rom_data, rom_size))
	{
		free(rom_data);
		return parsed_rom_data;
	}

	parsed_rom_data = ParseAmigaROMData(rom_data, rom_size, keyfile_path);

	if(parsed_rom_data.parsed_rom)
	{
		parsed_rom_data.rom_data = rom_data;
		if(parsed_rom_data.is_encrypted && parsed_rom_data.can_decrypt)
		{
			parsed_rom_data.rom_size = CryptAmigaROM(parsed_rom_data.rom_data, rom_size, false, keyfile_path);
			parsed_rom_data.successfully_decrypted = true;
		}
		else
		{
			parsed_rom_data.rom_size = rom_size;
		}
	}

	return parsed_rom_data;
}

// Parses and validates the data in the Amiga ROM and returns what is found
// in a ParsedAmigaROMData struct.
ParsedAmigaROMData ParseAmigaROMData(uint8_t *rom_contents, const size_t rom_size, const char *keyfile_path)
{
	uint8_t *rom_buffer;

	size_t rom_buffer_size = 0;
	int rom_encryption_result = 0;
	ParsedAmigaROMData return_data = {};

	return_data.parsed_rom = false;
	return_data.rom_data = NULL;
	return_data.rom_size = 0;
	return_data.is_encrypted = false;
	return_data.can_decrypt = false;
	return_data.successfully_decrypted = false;
	return_data.header = 'U';
	return_data.type = 'U';
	return_data.version = NULL;
	return_data.is_kickety_split = false;
	return_data.valid_footer = false;

	if(!ValidateAmigaROMSize(rom_contents, rom_size))
	{
		return return_data;
	}

	rom_encryption_result = DetectAmigaROMEncryption(rom_contents, rom_size);

	if(rom_encryption_result == -1)
	{
		return return_data;
	}
	else
	{
		return_data.is_encrypted = rom_encryption_result;
	}

	rom_buffer = (uint8_t*)malloc(rom_size * sizeof(uint8_t));
	if(!rom_buffer)
	{
		return return_data;
	}

	if(return_data.is_encrypted)
	{
		memcpy(rom_buffer, rom_contents, rom_size);

		rom_buffer_size = CryptAmigaROM(rom_buffer, rom_size, false, keyfile_path);

		if(rom_buffer_size == rom_size)
		{
			return_data.can_decrypt = false;
		}
		else
		{
			return_data.can_decrypt = true;
		}
	}
	else
	{
		return_data.can_decrypt = false;
	}

	if(!return_data.is_encrypted)
	{
		return_data.parsed_rom = true;
		return_data.rom_data = rom_contents;
		return_data.rom_size = rom_size;
		return_data.header = DetectAmigaKickstartROMTypeFromHeader(rom_contents, rom_size);
		return_data.type = DetectAmigaROMType(rom_contents, rom_size);
		return_data.version = DetectAmigaROMVersion(rom_contents, rom_size);
		return_data.is_kickety_split = DetectKicketySplitAmigaROM(rom_contents, rom_size);
		return_data.valid_footer = ValidateAmigaKickstartROMFooter(rom_contents, rom_size);
	}
	else if(return_data.is_encrypted && return_data.can_decrypt)
	{
		return_data.parsed_rom = true;
		return_data.rom_data = rom_contents;
		return_data.rom_size = rom_size;
		return_data.header = DetectAmigaKickstartROMTypeFromHeader(rom_buffer, rom_buffer_size);
		return_data.type = DetectAmigaROMType(rom_buffer, rom_buffer_size);
		return_data.version = DetectAmigaROMVersion(rom_buffer, rom_buffer_size);
		return_data.is_kickety_split = DetectKicketySplitAmigaROM(rom_buffer, rom_buffer_size);
		return_data.valid_footer = ValidateAmigaKickstartROMFooter(rom_buffer, rom_buffer_size);
	}

	free(rom_buffer);

	return return_data;
}

// Return whether an Amiga ROM is a valid size, accounting for
// encryption, if it's there.
bool ValidateAmigaROMSize(const uint8_t *rom_contents, const size_t rom_size)
{
	size_t actual_rom_size = rom_size;
	if(DetectAmigaROMEncryption(rom_contents, rom_size))
	{
		actual_rom_size = rom_size - 11;
	}

	return ((actual_rom_size > 10) && ((actual_rom_size & actual_rom_size - 1) == 0));
}

// Detects the version of the ROM by SHA1 hash
// Returns NULL for failure, else a string indicating the ROM version
const char* DetectAmigaROMVersion(const uint8_t *rom_contents, const size_t rom_size)
{
	uint8_t *digest;
	char *hexdigest;

	size_t i;
	size_t rom_quantity = sizeof(AMIGA_ROM_INFO) / sizeof(AmigaROMInfo);

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
	size_t rom_quantity = sizeof(AMIGA_ROM_INFO) / sizeof(AmigaROMInfo);

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

// Detect whether a ROM is a "Kickety-Split ROM"
// Returns true if it is, and false if it isn't.
bool DetectKicketySplitAmigaROM(const uint8_t *rom_contents, const size_t rom_size)
{
	const uint32_t *rom_contents_32 = (const uint32_t*)rom_contents;
	if(rom_size != 524288)
	{
		return false;
	}

	return (be32toh(rom_contents_32[131072]) == be32toh(AMIGA_256_ROM_HEADER));
}

// Detect which type of kickstart ROM a purported Kickstart ROM claims to be
// based on its header and size.  If this data is not consistent, or is unknown,
// the method returns 'U'.  Otherwise, it returns '5' for a 512k ROM, '2' for a
// 256k ROM, 'E' for an extended ROM, or 'R' for a "ReKick" ROM.
char DetectAmigaKickstartROMTypeFromHeader(const uint8_t *rom_contents, const size_t rom_size)
{
	const uint32_t *rom_contents_32 = (const uint32_t*)rom_contents;

	uint32_t amiga_256_header = be32toh(AMIGA_256_ROM_HEADER);
	uint32_t amiga_512_header = be32toh(AMIGA_512_ROM_HEADER);
	uint32_t amiga_ext_header = be32toh(AMIGA_EXT_ROM_HEADER);
	uint32_t amiga_rekick_rom_header = be32toh(AMIGA_512_REKICK_ROM_HEADER);
	uint32_t rom_header = be32toh(rom_contents_32[0]);

	if(rom_size == 524288)
	{
		if(rom_header == amiga_512_header)
		{
			return '5';
		}
	}
	else if(rom_size == 262144)
	{
		if(rom_header == amiga_256_header)
		{
			return '2';
		}
		else if(rom_header == amiga_ext_header)
		{
			return 'E';
		}
		else if(rom_header == amiga_rekick_rom_header)
		{
			return 'R';
		}
	}

	return 'U';
}

// Returns an unsigned 32-bit integer with the checksum for the ROM.
// This checksum is used by Amigas to validate whether a ROM is undamaged,
// and is located 24 bytes from the end of the ROM.
// If calc_new_sum is true, then the function will return a checksum value which
// will be accepted for use by an Amiga system.
uint32_t CalculateAmigaROMChecksum(const uint8_t *rom_contents, const size_t rom_size, const bool calc_new_sum)
{
	const uint32_t *rom_contents_32 = (const uint32_t*)rom_contents;

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
	const uint32_t *rom_contents_32 = (const uint32_t*)rom_contents;

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

// Validates whether an Amiga kickstart ROM as a valid footer.
// Returns true if it does, or false if it doesn't.
bool ValidateAmigaKickstartROMFooter(const uint8_t *rom_contents, const size_t rom_size)
{
	const uint16_t *rom_contents_16 = (const uint16_t*)rom_contents;

	uint16_t footer_value = 0x19;
	size_t i;

	for(i = (rom_size / 2) - 7; i < (rom_size / 2); i++)
	{
		if(be16toh(rom_contents_16[i]) != footer_value)
		{
			return false;
		}

		footer_value++;
	}

	return true;
}

// Detects whether an Amiga ROM is encrypted.
// Returns 1 if it is, 0 if it isn't, or -1 if it has an invalid size.
int DetectAmigaROMEncryption(const uint8_t *rom_contents, const size_t rom_size)
{
	if(rom_size < 11)
	{
		return -1;
	}
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

	int encryption_status = 0;
	bool is_encrypted = false;
	size_t keyfile_size = 0;
	size_t result_size = rom_size;

	encryption_status = DetectAmigaROMEncryption(rom_contents, rom_size);
	if(encryption_status == -1)
	{
		return rom_size;
	}

	is_encrypted = encryption_status;
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

	DoAmigaROMCryptOperation(result_buffer, result_size, keyfile_buffer, keyfile_size);

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

// Run the actual crypt operation, using the ROM data and keyfile data
void DoAmigaROMCryptOperation(uint8_t *rom_data_without_crypt_header, const size_t rom_size, const uint8_t *keyfile_data, const size_t keyfile_size)
{
	size_t i = 0;
	size_t key_idx = 0;

	for(i = 0; i < rom_size; i++)
	{
		rom_data_without_crypt_header[i] ^= keyfile_data[key_idx];
		key_idx = (key_idx + 1) % keyfile_size;
	}
}

// 0 indicates the ROM is not byte swapped (the ROM is for emulators)
// 1 indicates the ROM is byte swapped (the ROM is for physical ICs)
// -1 indicates the ROM is not an Amiga ROM known to this library
int DetectAmigaROMByteSwap(const uint8_t *rom_contents, const size_t rom_size)
{
	uint8_t *digest;
	char *hexdigest;

	size_t i;
	size_t rom_quantity = sizeof(AMIGA_ROM_INFO) / sizeof(AmigaROMInfo);

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
