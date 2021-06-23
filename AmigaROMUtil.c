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

	ParsedAmigaROMData amiga_rom = {};
	int seek_status;

	amiga_rom.parsed_rom = false;
	amiga_rom.rom_data = NULL;
	amiga_rom.rom_size = 0;
	amiga_rom.validated_size = false;
	amiga_rom.has_reset_vector = false;
	amiga_rom.is_encrypted = false;
	amiga_rom.can_decrypt = false;
	amiga_rom.successfully_decrypted = false;
	amiga_rom.is_byte_swapped = false;
	amiga_rom.has_valid_checksum = false;
	amiga_rom.header = 'U';
	amiga_rom.type = 'U';
	amiga_rom.version = NULL;
	amiga_rom.is_kickety_split = false;
	amiga_rom.valid_footer = false;

	if(!rom_file_path)
	{
		return amiga_rom;
	}

	fp = fopen(rom_file_path, "rb");
	if(!fp)
	{
		return amiga_rom;
	}

	seek_status = fseek(fp, 0, SEEK_END);

	if(seek_status < 0)
	{
		return amiga_rom;
	}
	else
	{
		amiga_rom.rom_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
	}

	amiga_rom.rom_data = (uint8_t*)malloc(amiga_rom.rom_size * sizeof(uint8_t));

	if(!amiga_rom.rom_data)
	{
		return amiga_rom;
	}

	fread(amiga_rom.rom_data, sizeof(uint8_t), amiga_rom.rom_size, fp);

	fclose(fp);

	ParseAmigaROMData(&amiga_rom, keyfile_path);

	if(amiga_rom.parsed_rom)
	{
		if(amiga_rom.is_encrypted && amiga_rom.can_decrypt)
		{
			if(CryptAmigaROM(&amiga_rom, false, keyfile_path))
			{
				amiga_rom.successfully_decrypted = true;
			}
		}
	}

	return amiga_rom;
}

// Puts ROM info data into output_string
void PrintAmigaROMInfo(const ParsedAmigaROMData *amiga_rom, char *output_string, const size_t string_length)
{
	snprintf(output_string, string_length, "ROM Info:\n\nSuccessfully parsed:\t\t%d\nROM size validated:\t\t%d\nHas reset vector:\t\t%d\nROM is encrypted:\t\t%d\nCan decrypt ROM:\t\t%d\nSuccessfully decrypted ROM:\t%d\nROM is byte swapped:\t\t%d\nROM has a valid checksum:\t%d\nROM header info:\t\t%c\nROM type:\t\t\t%c\nROM version:\t\t\t%s\nROM is a \"Kickety-Split\" ROM:\t%d\nROM has a valid footer:\t\t%d\n\n", amiga_rom->parsed_rom, amiga_rom->validated_size, amiga_rom->has_reset_vector, amiga_rom->is_encrypted, amiga_rom->can_decrypt, amiga_rom->successfully_decrypted, amiga_rom->is_byte_swapped, amiga_rom->has_valid_checksum, amiga_rom->header, amiga_rom->type, amiga_rom->version, amiga_rom->is_kickety_split, amiga_rom->valid_footer);
}

// Parses and validates the data in the Amiga ROM and returns what is found
// in a ParsedAmigaROMData struct.
void ParseAmigaROMData(ParsedAmigaROMData *amiga_rom, const char* keyfile_path)
{
	int rom_encryption_result = 0;

	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size == 0)
	{
		return;
	}

	amiga_rom->parsed_rom = false;
	amiga_rom->validated_size = false;
	amiga_rom->has_reset_vector = false;
	amiga_rom->is_encrypted = false;
	amiga_rom->can_decrypt = false;
	amiga_rom->successfully_decrypted = false;
	amiga_rom->is_byte_swapped = false;
	amiga_rom->has_valid_checksum = false;
	amiga_rom->header = 'U';
	amiga_rom->type = 'U';
	amiga_rom->version = NULL;
	amiga_rom->is_kickety_split = false;
	amiga_rom->valid_footer = false;

	if(!ValidateAmigaROMSize(amiga_rom))
	{
		return;
	}

	rom_encryption_result = DetectAmigaROMEncryption(amiga_rom);

	if(rom_encryption_result == -1)
	{
		return;
	}
	else
	{
		amiga_rom->is_encrypted = rom_encryption_result;
	}

	if(amiga_rom->is_encrypted)
	{
		if(!CryptAmigaROM(amiga_rom, false, keyfile_path))
		{
			amiga_rom->is_encrypted = true;
			amiga_rom->can_decrypt = false;
			amiga_rom->successfully_decrypted = false;
		}
		else
		{
			amiga_rom->is_encrypted = true;
			amiga_rom->can_decrypt = true;
			amiga_rom->successfully_decrypted = true;
		}
	}
	else
	{
		amiga_rom->can_decrypt = false;
	}

	if(!(amiga_rom->is_encrypted) || (amiga_rom->is_encrypted && amiga_rom->successfully_decrypted))
	{
		amiga_rom->parsed_rom = true;
		amiga_rom->validated_size = ValidateEmbeddedAmigaROMSize(amiga_rom);
		amiga_rom->has_reset_vector = ValidateAmigaROMResetVector(amiga_rom);
		amiga_rom->is_byte_swapped = (DetectAmigaROMByteSwap(amiga_rom) == 1);
		amiga_rom->has_valid_checksum = ValidateAmigaROMChecksum(amiga_rom);
		amiga_rom->header = DetectAmigaKickstartROMTypeFromHeader(amiga_rom);
		amiga_rom->type = DetectAmigaROMType(amiga_rom);
		amiga_rom->version = DetectAmigaROMVersion(amiga_rom);
		amiga_rom->is_kickety_split = DetectKicketySplitAmigaROM(amiga_rom);
		amiga_rom->valid_footer = ValidateAmigaKickstartROMFooter(amiga_rom);
	}
}

// Return whether an Amiga ROM is a valid size, accounting for
// encryption, if it's there.
bool ValidateAmigaROMSize(const ParsedAmigaROMData *amiga_rom)
{
	size_t actual_rom_size;

	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size == 0)
	{
		return false;
	}

	actual_rom_size = amiga_rom->rom_size;

	if(DetectAmigaROMEncryption(amiga_rom))
	{
		actual_rom_size = amiga_rom->rom_size - 11;
	}

	return ((actual_rom_size > 10) && ((actual_rom_size & actual_rom_size - 1) == 0));
}

// Validate that there is a reset vector (0x0x4E70) at 0x000000D0.
// Returns true if there is, or false if there isn't.
bool ValidateAmigaROMResetVector(const ParsedAmigaROMData *amiga_rom)
{
	const uint16_t *rom_data_16;

	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size == 0)
	{
		return false;
	}

	rom_data_16 = (const uint16_t*)(amiga_rom->rom_data);

	return (be16toh(rom_data_16[104]) == be16toh(0x4E70));
}

// Detects the version of the ROM by SHA1 hash
// Returns NULL for failure, else a string indicating the ROM version
const char* DetectAmigaROMVersion(const ParsedAmigaROMData *amiga_rom)
{
	uint8_t *digest;
	char *hexdigest;

	size_t i;
	size_t rom_quantity = sizeof(AMIGA_ROM_INFO) / sizeof(AmigaROMInfo);

	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size == 0)
	{
		return NULL;
	}

	digest = (uint8_t*)malloc(20 * sizeof(uint8_t));
	if(!digest)
	{
		return NULL;
	}

	hexdigest = (char*)malloc(41 * sizeof(char));
	if(!hexdigest)
	{
		free(digest);
		digest = NULL;
		return NULL;
	}

	if (sha1digest(digest, hexdigest, amiga_rom->rom_data, amiga_rom->rom_size))
	{
		free(digest);
		digest = NULL;
		free(hexdigest);
		hexdigest = NULL;
		return NULL;
	}

	for (i = 0; i < rom_quantity; i++)
	{
		if (strncmp(hexdigest, AMIGA_ROM_INFO[i].sha1hash, 40) == 0)
		{
			free(digest);
			digest = NULL;
			free(hexdigest);
			hexdigest = NULL;
			return AMIGA_ROM_INFO[i].version;
		}
	}

	free(digest);
	digest = NULL;
	free(hexdigest);
	hexdigest = NULL;
	return NULL;
}

// Returns a character indicating the type of ROM detected based on the SHA1 hash
// Valid return values are:
// A - Kickstart Hi/U34 ROM
// B - Kickstart Lo/U35 ROM
// M - Kickstart merged ROM
// O - Other (non-Kickstart) ROM
// U - Unknown ROM
char DetectAmigaROMType(const ParsedAmigaROMData *amiga_rom)
{
	uint8_t *digest;
	char *hexdigest;

	size_t i;
	size_t rom_quantity = sizeof(AMIGA_ROM_INFO) / sizeof(AmigaROMInfo);

	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size == 0)
	{
		return 'U';
	}

	digest = (uint8_t*)malloc(20 * sizeof(uint8_t));
	if(!digest)
	{
		return 'U';
	}

	hexdigest = (char*)malloc(41 * sizeof(char));
	if(!hexdigest)
	{
		free(digest);
		digest = NULL;
		return 'U';
	}

	if (sha1digest(digest, hexdigest, amiga_rom->rom_data, amiga_rom->rom_size))
	{
		free(digest);
		digest = NULL;
		free(hexdigest);
		hexdigest = NULL;
		return 'U';
	}

	for (i = 0; i < rom_quantity; i++)
	{
		if (strncmp(hexdigest, AMIGA_ROM_INFO[i].sha1hash, 40) == 0)
		{
			free(digest);
			digest = NULL;
			free(hexdigest);
			hexdigest = NULL;
			return AMIGA_ROM_INFO[i].type;
		}
	}

	free(digest);
	digest = NULL;
	free(hexdigest);
	hexdigest = NULL;
	return 'U';
}

// Detect whether a ROM is a "Kickety-Split ROM"
// Returns true if it is, and false if it isn't.
bool DetectKicketySplitAmigaROM(const ParsedAmigaROMData *amiga_rom)
{
	const uint32_t *rom_data_32;

	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size == 0)
	{
		return false;
	}

	rom_data_32 = (const uint32_t*)(amiga_rom->rom_data);

	if(amiga_rom->rom_size != 524288)
	{
		return false;
	}

	return (be32toh(rom_data_32[131072]) == be32toh(AMIGA_256_ROM_HEADER));
}

// Detect which type of kickstart ROM a purported Kickstart ROM claims to be
// based on its header and size.  If this data is not consistent, or is unknown,
// the method returns 'U'.  Otherwise, it returns '5' for a 512k ROM, '2' for a
// 256k ROM, 'E' for an extended ROM, or 'R' for a "ReKick" ROM.
char DetectAmigaKickstartROMTypeFromHeader(const ParsedAmigaROMData *amiga_rom)
{
	const uint32_t *rom_data_32;

	uint32_t amiga_256_header = be32toh(AMIGA_256_ROM_HEADER);
	uint32_t amiga_512_header = be32toh(AMIGA_512_ROM_HEADER);
	uint32_t amiga_ext_header = be32toh(AMIGA_EXT_ROM_HEADER);
	uint32_t amiga_rekick_rom_header = be32toh(AMIGA_512_REKICK_ROM_HEADER);
	uint32_t rom_header;

	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size == 0)
	{
		return 'U';
	}

	rom_data_32 = (const uint32_t*)(amiga_rom->rom_data);

	if(amiga_rom->rom_size < 4)
	{
		return 'U';
	}

	rom_header = be32toh(rom_data_32[0]);

	if(amiga_rom->rom_size == 524288)
	{
		if(rom_header == amiga_512_header)
		{
			return '5';
		}
	}
	else if(amiga_rom->rom_size == 262144)
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
uint32_t CalculateAmigaROMChecksum(const ParsedAmigaROMData *amiga_rom, const bool calc_new_sum)
{
	const uint32_t *rom_data_32;

	uint32_t sum = 0;
	uint32_t temp = 0;
	size_t i = 0;

	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size == 0)
	{
		return 0xFFFFFFFF;
	}

	rom_data_32 = (const uint32_t*)(amiga_rom->rom_data);

	if(amiga_rom->rom_size < 24)
	{
		return 0xFFFFFFFF;
	}

	for(i = 0; i < amiga_rom->rom_size / 4; i++)
	{
		if(calc_new_sum)
		{
			if(i != (amiga_rom->rom_size - 24) / 4)
			{
				temp = be32toh(rom_data_32[i]);
			}
			else
			{
				temp = 0;
			}
		}
		else
		{
			temp = be32toh(rom_data_32[i]);
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
uint32_t GetEmbeddedAmigaROMChecksum(const ParsedAmigaROMData *amiga_rom)
{
	const uint32_t *rom_data_32 = (const uint32_t*)(amiga_rom->rom_data);

	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size == 0)
	{
		return 0;
	}

	if(amiga_rom->rom_size < 24)
	{
		return 0;
	}

	return be32toh(rom_data_32[(amiga_rom->rom_size - 24) / 4]);
}

// Returns a boolean indicating whether the calculated checksum in the ROM
// matches the calculated checksum for the ROM.
bool ValidateAmigaROMChecksum(const ParsedAmigaROMData *amiga_rom)
{
	uint32_t calculated_sum;

	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size == 0)
	{
		return false;
	}

	calculated_sum = CalculateAmigaROMChecksum(amiga_rom, false);

	return (calculated_sum == 0);
}

// Calculates and embeds a correct checksum in the ROM.
// Returns true if it succeeds, or false if it fails.
bool CorrectAmigaROMChecksum(ParsedAmigaROMData *amiga_rom)
{
	uint32_t *rom_data_32;
	uint8_t *temp_rom_data;

	uint32_t old_sum = 0;
	uint32_t new_sum = 0;

	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size == 0)
	{
		return false;
	}

	temp_rom_data = (uint8_t*)malloc(amiga_rom->rom_size * sizeof(uint8_t));
	if(!temp_rom_data)
	{
		return false;
	}

	memcpy(temp_rom_data, amiga_rom->rom_data, amiga_rom->rom_size);

	rom_data_32 = (uint32_t*)temp_rom_data;
	old_sum = be32toh(amiga_rom->rom_data[(amiga_rom->rom_size - 24) / 4]);
	new_sum = CalculateAmigaROMChecksum(amiga_rom, true);

	if(old_sum == new_sum)
	{
		free(temp_rom_data);
		temp_rom_data = NULL;
		return true;
	}

	if(new_sum == 0)
	{
		free(temp_rom_data);
		temp_rom_data = NULL;
		return false;
	}

	rom_data_32[(amiga_rom->rom_size - 24) / 4] = htobe32(new_sum);

	if(!ValidateAmigaROMChecksum(amiga_rom))
	{
		free(temp_rom_data);
		temp_rom_data = NULL;
		return false;
	}

	memcpy(amiga_rom->rom_data, temp_rom_data, amiga_rom->rom_size);

	amiga_rom->has_valid_checksum = true;

	free(temp_rom_data);
	temp_rom_data = NULL;
	return true;
}

// Validates whether an Amiga kickstart ROM as a valid footer.
// Returns true if it does, or false if it doesn't.
bool ValidateAmigaKickstartROMFooter(const ParsedAmigaROMData *amiga_rom)
{
	const uint16_t *rom_data_16;

	uint16_t footer_value = 0x19;
	size_t i;

	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size == 0)
	{
		return false;
	}

	rom_data_16 = (const uint16_t*)(amiga_rom->rom_data);

	for(i = (amiga_rom->rom_size / 2) - 7; i < (amiga_rom->rom_size / 2); i++)
	{
		if(be16toh(rom_data_16[i]) != footer_value)
		{
			return false;
		}

		footer_value++;
	}

	return true;
}

// Validate the ROM size matches the size embedded in the ROM.
// Returns true if it does, or false if it doesn't.
bool ValidateEmbeddedAmigaROMSize(const ParsedAmigaROMData *amiga_rom)
{
	const uint32_t *rom_data_32;

	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size == 0)
	{
		return false;
	}

	rom_data_32 = (const uint32_t*)(amiga_rom->rom_data);
	return (be32toh(rom_data_32[(amiga_rom->rom_size / 4) - 5]) == amiga_rom->rom_size);
}

// Detects whether an Amiga ROM is encrypted.
// Returns 1 if it is, 0 if it isn't, or -1 if it has an invalid size.
int DetectAmigaROMEncryption(const ParsedAmigaROMData *amiga_rom)
{
	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size == 0)
	{
		return -1;
	}

	if(amiga_rom->rom_size < 11)
	{
		return -1;
	}
	return (strncmp((char*)(amiga_rom->rom_data), "AMIROMTYPE1", 11) == 0);
}

// Encrypts or decrypts Amiga ROMs according to whether crypt_operation is true or false.
// If true, it will encrypt the ROMs.  If false, it will decrypt them.
// The function returns true if the method succeeds, and false if it fails.
// For safety, make sure that rom_contents has space for 11 more bytes than the original
// ROM had if an encrypt operation is performed.
bool CryptAmigaROM(ParsedAmigaROMData *amiga_rom, const bool crypt_operation, const char *keyfile_path)
{
	FILE *fp;
	uint8_t *keyfile_buffer;
	uint8_t *result_buffer;

	int is_encrypted = 0;
	size_t keyfile_size = 0;
	size_t result_size = amiga_rom->rom_size;
	int seek_status;

	if(!amiga_rom || !keyfile_path || !(amiga_rom->rom_data) || (amiga_rom->rom_size == 0 && crypt_operation) || (amiga_rom->rom_size < 11 && !crypt_operation))
	{
		return false;
	}

	is_encrypted = DetectAmigaROMEncryption(amiga_rom);
	if(is_encrypted == -1)
	{
		return false;
	}

	if(is_encrypted == crypt_operation)
	{
		return false;
	}

	if(crypt_operation)
	{
		result_buffer = (uint8_t*)malloc((amiga_rom->rom_size + 11) * sizeof(uint8_t));
	}
	else
	{
		result_buffer = (uint8_t*)malloc((amiga_rom->rom_size - 11) * sizeof(uint8_t));
	}

	if(!result_buffer)
	{
		return false;
	}

	fp = fopen(keyfile_path, "rb");
	if(!fp)
	{
		free(result_buffer);
		result_buffer = NULL;
		return false;
	}

	seek_status = fseek(fp, 0, SEEK_END);

	if(seek_status < 0)
	{
		free(result_buffer);
		result_buffer = NULL;
		return false;
	}
	else
	{
		keyfile_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
	}

	keyfile_buffer = (uint8_t*)malloc(keyfile_size * sizeof(uint8_t));
	if(!keyfile_buffer)
	{
		free(result_buffer);
		result_buffer = NULL;
		return false;
	}

	fread(keyfile_buffer, sizeof(uint8_t), keyfile_size, fp);

	fclose(fp);

	if(keyfile_size < 1)
	{
		free(keyfile_buffer);
		keyfile_buffer = NULL;
		free(result_buffer);
		result_buffer = NULL;
		return false;
	}

	if(is_encrypted)
	{
		result_size = result_size - 11;
		memcpy(result_buffer, &(amiga_rom->rom_data)[11], result_size);
	}
	else
	{
		memcpy(result_buffer, amiga_rom->rom_data, result_size);
	}

	if(!DoAmigaROMCryptOperation(result_buffer, result_size, keyfile_buffer, keyfile_size))
	{
		free(keyfile_buffer);
		keyfile_buffer = NULL;
		free(result_buffer);
		result_buffer = NULL;
		return false;
	}

	if(is_encrypted)
	{
		amiga_rom->rom_data = realloc(amiga_rom->rom_data, result_size * sizeof(uint8_t));
		if(!amiga_rom->rom_data)
		{
			free(keyfile_buffer);
			keyfile_buffer = NULL;
			free(result_buffer);
			result_buffer = NULL;
			return false;
		}

		memcpy(amiga_rom->rom_data, result_buffer, result_size);
		amiga_rom->rom_size = result_size;
	}
	else
	{
		amiga_rom->rom_data = realloc(amiga_rom->rom_data, (result_size + 11) * sizeof(uint8_t));
		if(!amiga_rom->rom_data)
		{
			free(keyfile_buffer);
			keyfile_buffer = NULL;
			free(result_buffer);
			result_buffer = NULL;
			return false;
		}

		snprintf((char*)(amiga_rom->rom_data), 12, "AMIROMTYPE1");
		memcpy(&(amiga_rom->rom_data)[11], result_buffer, result_size);
		amiga_rom->rom_size = result_size + 11;

		ParseAmigaROMData(amiga_rom, NULL);
	}

	free(keyfile_buffer);
	keyfile_buffer = NULL;
	free(result_buffer);
	result_buffer = NULL;

	return true;
}

// Run the actual crypt operation, using the ROM data and keyfile data
bool DoAmigaROMCryptOperation(uint8_t *rom_data_without_crypt_header, const size_t rom_size, const uint8_t *keyfile_data, const size_t keyfile_size)
{
	size_t i = 0;
	size_t key_idx = 0;

	if(!rom_data_without_crypt_header || rom_size == 0 || !keyfile_data || keyfile_size == 0)
	{
		return false;
	}

	for(i = 0; i < rom_size; i++)
	{
		rom_data_without_crypt_header[i] ^= keyfile_data[key_idx];
		key_idx = (key_idx + 1) % keyfile_size;
	}

	return true;
}

// 0 indicates the ROM is not byte swapped (the ROM is for emulators)
// 1 indicates the ROM is byte swapped (the ROM is for physical ICs)
// -1 indicates the ROM is not an Amiga ROM known to this library
int DetectAmigaROMByteSwap(const ParsedAmigaROMData *amiga_rom)
{
	uint8_t *digest;
	char *hexdigest;

	size_t i;
	size_t rom_quantity = sizeof(AMIGA_ROM_INFO) / sizeof(AmigaROMInfo);

	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size == 0)
	{
		return -1;
	}

	digest = (uint8_t*)malloc(20 * sizeof(uint8_t));
	if(!digest)
	{
		return -1;
	}

	hexdigest = (char*)malloc(41 * sizeof(char));
	if(!hexdigest)
	{
		free(digest);
		digest = NULL;
		return -1;
	}

	if (sha1digest(digest, hexdigest, amiga_rom->rom_data, amiga_rom->rom_size))
	{
		free(digest);
		digest = NULL;
		free(hexdigest);
		hexdigest = NULL;
		return -1;
	}

	for (i = 0; i < rom_quantity; i++)
	{
		if (strncmp(hexdigest, AMIGA_ROM_INFO[i].sha1hash, 40) == 0)
		{
			free(digest);
			digest = NULL;
			free(hexdigest);
			hexdigest = NULL;
			return AMIGA_ROM_INFO[i].byte_swap;
		}
	}

	free(digest);
	digest = NULL;
	free(hexdigest);
	hexdigest = NULL;
	return -1;
}

// If swap_unconditionally is true, the method will swap the ROM's bytes regardless of whether
// or not it is a known ROM.
// Returns true for success or false for failure.
bool SetAmigaROMByteSwap(ParsedAmigaROMData *amiga_rom, const bool swap_bytes, const bool unswap_bytes, const bool swap_unconditionally)
{
	size_t i;
	int is_swapped;

	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size == 0)
	{
		return false;
	}

	if(swap_bytes && unswap_bytes && !swap_unconditionally)
	{
		return false;
	}

	if(!swap_unconditionally)
	{
		is_swapped = DetectAmigaROMByteSwap(amiga_rom);
		if (is_swapped < 0)
		{
			return false;
		}
	}

	if (swap_unconditionally || (is_swapped == 0 && swap_bytes) || (is_swapped == 1 && unswap_bytes))
	{
		for (i = 0; i < amiga_rom->rom_size; i += 2)
		{
			// In-place XOR swap of two bytes
			(amiga_rom->rom_data)[i] ^= (amiga_rom->rom_data)[i + 1];
			(amiga_rom->rom_data)[i + 1] ^= (amiga_rom->rom_data)[i];
			(amiga_rom->rom_data)[i] ^= (amiga_rom->rom_data)[i + 1];
		}

		amiga_rom->is_byte_swapped = !(amiga_rom->is_byte_swapped);
		return true;
	}

	return false;
}

// For this method, ROM A and ROM B should each be the same size as the merged ROM.
// Each A and B ROM gets the same contents repeated twice.
// Returns true if it succeeds, or false if it doesn't.
bool SplitAmigaROM(const ParsedAmigaROMData *amiga_rom, uint8_t *rom_high_contents, uint8_t *rom_low_contents)
{
	size_t i;

	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size == 0 || amiga_rom->rom_size % 2 != 0 || !rom_high_contents || !rom_low_contents)
	{
		return false;
	}

	for(i = 0; i < amiga_rom->rom_size; i = i + 4)
	{
		memcpy(&rom_high_contents[i / 2], &(amiga_rom->rom_data)[i], 2);
		memcpy(&rom_low_contents[i / 2], &(amiga_rom->rom_data)[i + 2], 2);
	}

	memcpy(&rom_high_contents[amiga_rom->rom_size / 2], &rom_high_contents[0], amiga_rom->rom_size / 2);
	memcpy(&rom_low_contents[amiga_rom->rom_size / 2], &rom_low_contents[0], amiga_rom->rom_size / 2);

	return true;
}

// For this method, ROM A and ROM B should each be the same size as the merged ROM.
// Each A and B ROM gets the same contents repeated twice.
// Returns true if it succeeds, or false if it doesn't.
bool MergeAmigaROM(const uint8_t *rom_high_contents, const uint8_t *rom_low_contents, const size_t split_rom_size, ParsedAmigaROMData *amiga_rom)
{
	size_t i;

	if(!amiga_rom || !(amiga_rom->rom_data) || !rom_high_contents || !rom_low_contents || split_rom_size == 0 || split_rom_size % 4 != 0)
	{
		return false;
	}

	for(i = 0; i < split_rom_size / 4; i++)
	{
		memcpy(&(amiga_rom->rom_data)[i * 4], &rom_high_contents[i * 2], 2);
		memcpy(&(amiga_rom->rom_data)[(i * 4) + 2], &rom_low_contents[i * 2], 2);
	}

	amiga_rom->rom_size = split_rom_size;

	ParseAmigaROMData(amiga_rom, NULL);

	return true;
}
