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
#include "AmigaROMMajorMinorVersions.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

#define AMIGA_256_ROM_HEADER                 0x11114EF9
#define AMIGA_256_ROM_HEADER_BYTESWAP        0x1111F94E
#define AMIGA_512_ROM_HEADER                 0x11144EF9
#define AMIGA_512_ROM_HEADER_BYTESWAP        0x1411F94E
#define AMIGA_EXT_ROM_HEADER                 0x11144EF9
#define AMIGA_EXT_ROM_HEADER_BYTESWAP        0x1411F94E
#define AMIGA_512_REKICK_ROM_HEADER          0x11164EF9 //TODO: Properly detect size/handle these
#define AMIGA_512_REKICK_ROM_HEADER_BYTESWAP 0x1611F94E //TODO: Properly detect size/handle these

extern int sha1digest(uint8_t *digest, char *hexdigest, const uint8_t *data, size_t databytes);

// Create and return a new and initialized struct.
// Pointers are NOT allocated, but are NULL instead.
ParsedAmigaROMData GetInitializedAmigaROM(void)
{
	ParsedAmigaROMData amiga_rom;

	amiga_rom.is_initialized = true;
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
	amiga_rom.header = 0;
	amiga_rom.type = 'U';
	amiga_rom.version = NULL;
	amiga_rom.major_version = 0;
	amiga_rom.minor_version = 0xffff;
	amiga_rom.major_minor_version = NULL;
	amiga_rom.is_kickety_split = false;
	amiga_rom.valid_footer = false;

	return amiga_rom;
}

// Free all pointers which are expected to potentially be
// allocated in a struct and sets the pointers to NULL,
// and sets the rest of the struct to default values.
void DestroyInitializedAmigaROM(ParsedAmigaROMData *amiga_rom)
{
	amiga_rom->is_initialized = false;
	amiga_rom->parsed_rom = false;

	if(amiga_rom->rom_data)
	{
		free(amiga_rom->rom_data);
		amiga_rom->rom_data = NULL;
	}

	amiga_rom->rom_size = 0;
	amiga_rom->validated_size = false;
	amiga_rom->has_reset_vector = false;
	amiga_rom->is_encrypted = false;
	amiga_rom->can_decrypt = false;
	amiga_rom->successfully_decrypted = false;
	amiga_rom->is_byte_swapped = false;
	amiga_rom->has_valid_checksum = false;
	amiga_rom->header = 0;
	amiga_rom->type = 'U';
	amiga_rom->version = NULL;
	amiga_rom->major_version = 0;
	amiga_rom->minor_version = 0xffff;
	amiga_rom->major_minor_version = NULL;
	amiga_rom->is_kickety_split = false;
	amiga_rom->valid_footer = false;
}

// Returns a parsed ROM data struct, with the ROM data and size included.
// If anything files, parsed_rom will be false.  If encrypted, the ROM
// will be decrypted.
ParsedAmigaROMData ReadAmigaROM(const char *rom_file_path, const char *keyfile_path)
{
	FILE *fp;

	ParsedAmigaROMData amiga_rom = GetInitializedAmigaROM();
	int seek_status;

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
		fclose(fp);
		return amiga_rom;
	}
	else
	{
		amiga_rom.rom_size = (size_t)ftell(fp);
		fseek(fp, 0, SEEK_SET);
	}

	amiga_rom.rom_data = (uint8_t*)malloc(amiga_rom.rom_size);

	if(!amiga_rom.rom_data)
	{
		fclose(fp);
		return amiga_rom;
	}

	fread(amiga_rom.rom_data, 1, amiga_rom.rom_size, fp);

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

// Detect whether a ROM is an Amiga kickstart ROM based on size, header, reset vector,
// magic, and footer.
bool IsAmigaROM(const ParsedAmigaROMData *amiga_rom)
{
	if(DetectAmigaKickstartROMTypeFromHeader(amiga_rom) == 0)
	{
		return false;
	}

	if(!ValidateAmigaROMResetVector(amiga_rom))
	{
		return false;
	}

	if(!ValidateAmigaKickstartROMFooter(amiga_rom))
	{
		return false;
	}

	return true;
}

// Puts ROM info data into output_string
void PrintAmigaROMInfo(const ParsedAmigaROMData *amiga_rom, char *output_string, const size_t string_length)
{
	char *successfully_parsed = NULL;
	char *rom_size_validated = NULL;
	char *has_reset_vector = NULL;
	char *rom_is_encrypted = NULL;
	char *can_decrypt_rom = NULL;
	char *successfully_decrypted_rom = NULL;
	char *rom_is_byte_swapped = NULL;
	char *rom_has_valid_checksum = NULL;
	char *rom_header_info = NULL;
	char *rom_type = NULL;
	char *rom_version = NULL;
	char *embedded_rom_major_version = NULL;
	char *embedded_rom_minor_version = NULL;
	char *detected_embedded_rom_version = NULL;
	char *is_kickety_split = NULL;
	char *has_valid_footer = NULL;

	if(!amiga_rom || !output_string)
	{
		return;
	}

	successfully_parsed = (char *)malloc(64);
	if(!successfully_parsed)
	{
		return;
	}
	snprintf(successfully_parsed, 64, "Successfully parsed:\t\t%d", amiga_rom->parsed_rom);

	rom_size_validated = (char *)malloc(64);
	if(!rom_size_validated)
	{
		free(successfully_parsed);
		successfully_parsed = NULL;
		return;
	}
	snprintf(rom_size_validated, 64, "ROM size validated:\t\t%d", amiga_rom->validated_size);

	has_reset_vector = (char *)malloc(64);
	if(!has_reset_vector)
	{
		free(successfully_parsed);
		successfully_parsed = NULL;
		free(rom_size_validated);
		rom_size_validated = NULL;
		return;
	}
	snprintf(has_reset_vector, 64, "Has reset vector:\t\t%d", amiga_rom->has_reset_vector);

	rom_is_encrypted = (char *)malloc(64);
	if(!rom_is_encrypted)
	{
		free(successfully_parsed);
		successfully_parsed = NULL;
		free(rom_size_validated);
		rom_size_validated = NULL;
		free(has_reset_vector);
		has_reset_vector = NULL;
		return;
	}
	snprintf(rom_is_encrypted, 64, "ROM is encrypted:\t\t%d", amiga_rom->is_encrypted);

	can_decrypt_rom = (char *)malloc(64);
	if(!can_decrypt_rom)
	{
		free(successfully_parsed);
		successfully_parsed = NULL;
		free(rom_size_validated);
		rom_size_validated = NULL;
		free(has_reset_vector);
		has_reset_vector = NULL;
		free(rom_is_encrypted);
		rom_is_encrypted = NULL;
		return;
	}
	snprintf(can_decrypt_rom, 64, "Can decrypt ROM:\t\t%d", amiga_rom->can_decrypt);

	successfully_decrypted_rom = (char *)malloc(64);
	if(!successfully_decrypted_rom)
	{
		free(successfully_parsed);
		successfully_parsed = NULL;
		free(rom_size_validated);
		rom_size_validated = NULL;
		free(has_reset_vector);
		has_reset_vector = NULL;
		free(rom_is_encrypted);
		rom_is_encrypted = NULL;
		free(can_decrypt_rom);
		can_decrypt_rom = NULL;
		return;
	}
	snprintf(successfully_decrypted_rom, 64, "Successfully decrypted ROM:\t%d", amiga_rom->successfully_decrypted);

	rom_is_byte_swapped = (char *)malloc(64);
	if(!rom_is_byte_swapped)
	{
		free(successfully_parsed);
		successfully_parsed = NULL;
		free(rom_size_validated);
		rom_size_validated = NULL;
		free(has_reset_vector);
		has_reset_vector = NULL;
		free(rom_is_encrypted);
		rom_is_encrypted = NULL;
		free(can_decrypt_rom);
		can_decrypt_rom = NULL;
		free(successfully_decrypted_rom);
		successfully_decrypted_rom = NULL;
		return;
	}
	snprintf(rom_is_byte_swapped, 64, "ROM is byte swapped:\t\t%d", amiga_rom->is_byte_swapped);

	rom_has_valid_checksum = (char *)malloc(64);
	if(!rom_has_valid_checksum)
	{
		free(successfully_parsed);
		successfully_parsed = NULL;
		free(rom_size_validated);
		rom_size_validated = NULL;
		free(has_reset_vector);
		has_reset_vector = NULL;
		free(rom_is_encrypted);
		rom_is_encrypted = NULL;
		free(can_decrypt_rom);
		can_decrypt_rom = NULL;
		free(successfully_decrypted_rom);
		successfully_decrypted_rom = NULL;
		free(rom_is_byte_swapped);
		rom_is_byte_swapped = NULL;
		return;
	}
	snprintf(rom_has_valid_checksum, 64, "ROM has a valid checksum:\t%d", amiga_rom->has_valid_checksum);

	rom_header_info = (char *)malloc(64);
	if(!rom_header_info)
	{
		free(successfully_parsed);
		successfully_parsed = NULL;
		free(rom_size_validated);
		rom_size_validated = NULL;
		free(has_reset_vector);
		has_reset_vector = NULL;
		free(rom_is_encrypted);
		rom_is_encrypted = NULL;
		free(can_decrypt_rom);
		can_decrypt_rom = NULL;
		free(successfully_decrypted_rom);
		successfully_decrypted_rom = NULL;
		free(rom_is_byte_swapped);
		rom_is_byte_swapped = NULL;
		free(rom_has_valid_checksum);
		rom_has_valid_checksum = NULL;
		return;
	}

	switch(amiga_rom->header)
	{
		case 0:
			snprintf(rom_header_info, 64, "ROM type indicated by header:\tNot an Amiga ROM");
			break;
		case 1:
			snprintf(rom_header_info, 64, "ROM type indicated by header:\t256KB Kickstart ROM");
			break;
		case 2:
			snprintf(rom_header_info, 64, "ROM type indicated by header:\t512KB Kickstart ROM");
			break;
		case 3:
			snprintf(rom_header_info, 64, "ROM type indicated by header:\tExtended ROM");
			break;
		case 4:
			snprintf(rom_header_info, 64, "ROM type indicated by header:\t\"ReKick\" ROM");
			break;
		case 5:
			snprintf(rom_header_info, 64, "ROM type indicated by header:\tAmbiguous");
			break;
		default:
			snprintf(rom_header_info, 64, "ROM type indicated by header:\tUnknown");
			break;
	}

	rom_type = (char *)malloc(64);
	if(!rom_type)
	{
		free(successfully_parsed);
		successfully_parsed = NULL;
		free(rom_size_validated);
		rom_size_validated = NULL;
		free(has_reset_vector);
		has_reset_vector = NULL;
		free(rom_is_encrypted);
		rom_is_encrypted = NULL;
		free(can_decrypt_rom);
		can_decrypt_rom = NULL;
		free(successfully_decrypted_rom);
		successfully_decrypted_rom = NULL;
		free(rom_is_byte_swapped);
		rom_is_byte_swapped = NULL;
		free(rom_has_valid_checksum);
		rom_has_valid_checksum = NULL;
		free(rom_header_info);
		rom_header_info = NULL;
		return;
	}

	switch(amiga_rom->type)
	{
		case 'A':
			snprintf(rom_type, 64, "ROM type:\t\t\tKickstart Hi ROM");
			break;
		case 'B':
			snprintf(rom_type, 64, "ROM type:\t\t\tKickstart Lo ROM");
			break;
		case 'E':
			snprintf(rom_type, 64, "ROM type:\t\t\tExtended Amiga ROM");
			break;
		case 'M':
			snprintf(rom_type, 64, "ROM type:\t\t\tMerged Kickstart ROM");
			break;
		case 'O':
			snprintf(rom_type, 64, "ROM type:\t\t\tNon-Kickstart Amiga ROM");
			break;
		case 'U':
		default:
			snprintf(rom_type, 64, "ROM type:\t\t\tUnknown ROM");
			break;
	}

	rom_version = (char *)malloc(143);
	if(!rom_version)
	{
		free(successfully_parsed);
		successfully_parsed = NULL;
		free(rom_size_validated);
		rom_size_validated = NULL;
		free(has_reset_vector);
		has_reset_vector = NULL;
		free(rom_is_encrypted);
		rom_is_encrypted = NULL;
		free(can_decrypt_rom);
		can_decrypt_rom = NULL;
		free(successfully_decrypted_rom);
		successfully_decrypted_rom = NULL;
		free(rom_is_byte_swapped);
		rom_is_byte_swapped = NULL;
		free(rom_has_valid_checksum);
		rom_has_valid_checksum = NULL;
		free(rom_header_info);
		rom_header_info = NULL;
		free(rom_type);
		rom_type = NULL;
		return;
	}
	snprintf(rom_version, 143, "ROM version:\t\t\t%s", amiga_rom->version);

	embedded_rom_major_version = (char *)malloc(64);
	if(!embedded_rom_major_version)
	{
		free(successfully_parsed);
		successfully_parsed = NULL;
		free(rom_size_validated);
		rom_size_validated = NULL;
		free(has_reset_vector);
		has_reset_vector = NULL;
		free(rom_is_encrypted);
		rom_is_encrypted = NULL;
		free(can_decrypt_rom);
		can_decrypt_rom = NULL;
		free(successfully_decrypted_rom);
		successfully_decrypted_rom = NULL;
		free(rom_is_byte_swapped);
		rom_is_byte_swapped = NULL;
		free(rom_has_valid_checksum);
		rom_has_valid_checksum = NULL;
		free(rom_header_info);
		rom_header_info = NULL;
		free(rom_type);
		rom_type = NULL;
		free(rom_version);
		rom_version = NULL;
		return;
	}

	if(amiga_rom->major_version != 0xffff)
	{
		snprintf(embedded_rom_major_version, 64, "ROM major version number:\t%u", amiga_rom->major_version);
	}
	else
	{
		snprintf(embedded_rom_major_version, 64, "ROM major version number:\tUnknown");
	}

	embedded_rom_minor_version = (char *)malloc(64);
	if(!embedded_rom_minor_version)
	{
		free(successfully_parsed);
		successfully_parsed = NULL;
		free(rom_size_validated);
		rom_size_validated = NULL;
		free(has_reset_vector);
		has_reset_vector = NULL;
		free(rom_is_encrypted);
		rom_is_encrypted = NULL;
		free(can_decrypt_rom);
		can_decrypt_rom = NULL;
		free(successfully_decrypted_rom);
		successfully_decrypted_rom = NULL;
		free(rom_is_byte_swapped);
		rom_is_byte_swapped = NULL;
		free(rom_has_valid_checksum);
		rom_has_valid_checksum = NULL;
		free(rom_header_info);
		rom_header_info = NULL;
		free(rom_type);
		rom_type = NULL;
		free(rom_version);
		rom_version = NULL;
		free(embedded_rom_major_version);
		embedded_rom_major_version = NULL;
		return;
	}

	if(amiga_rom->minor_version != 0xffff)
	{
		snprintf(embedded_rom_minor_version, 64, "ROM minor version number:\t%u", amiga_rom->minor_version);
	}
	else
	{
		snprintf(embedded_rom_minor_version, 64, "ROM minor version number:\tN/A");
	}

	detected_embedded_rom_version = (char *)malloc(157);
	if(!detected_embedded_rom_version)
	{
		free(successfully_parsed);
		successfully_parsed = NULL;
		free(rom_size_validated);
		rom_size_validated = NULL;
		free(has_reset_vector);
		has_reset_vector = NULL;
		free(rom_is_encrypted);
		rom_is_encrypted = NULL;
		free(can_decrypt_rom);
		can_decrypt_rom = NULL;
		free(successfully_decrypted_rom);
		successfully_decrypted_rom = NULL;
		free(rom_is_byte_swapped);
		rom_is_byte_swapped = NULL;
		free(rom_has_valid_checksum);
		rom_has_valid_checksum = NULL;
		free(rom_header_info);
		rom_header_info = NULL;
		free(rom_type);
		rom_type = NULL;
		free(rom_version);
		rom_version = NULL;
		free(embedded_rom_major_version);
		embedded_rom_major_version = NULL;
		free(embedded_rom_minor_version);
		embedded_rom_minor_version = NULL;
		return;
	}

	snprintf(detected_embedded_rom_version, 157, "ROM detected version number:\t%s", amiga_rom->major_minor_version);

	is_kickety_split = (char *)malloc(64);
	if(!is_kickety_split)
	{
		free(successfully_parsed);
		successfully_parsed = NULL;
		free(rom_size_validated);
		rom_size_validated = NULL;
		free(has_reset_vector);
		has_reset_vector = NULL;
		free(rom_is_encrypted);
		rom_is_encrypted = NULL;
		free(can_decrypt_rom);
		can_decrypt_rom = NULL;
		free(successfully_decrypted_rom);
		successfully_decrypted_rom = NULL;
		free(rom_is_byte_swapped);
		rom_is_byte_swapped = NULL;
		free(rom_has_valid_checksum);
		rom_has_valid_checksum = NULL;
		free(rom_header_info);
		rom_header_info = NULL;
		free(rom_type);
		rom_type = NULL;
		free(rom_version);
		rom_version = NULL;
		free(embedded_rom_major_version);
		embedded_rom_major_version = NULL;
		free(embedded_rom_minor_version);
		embedded_rom_minor_version = NULL;
		free(detected_embedded_rom_version);
		detected_embedded_rom_version = NULL;
		return;
	}

	snprintf(is_kickety_split, 64, "ROM kickety split status:\t%d", amiga_rom->is_kickety_split);

	has_valid_footer = (char *)malloc(64);
	if(!has_valid_footer)
	{
		free(successfully_parsed);
		successfully_parsed = NULL;
		free(rom_size_validated);
		rom_size_validated = NULL;
		free(has_reset_vector);
		has_reset_vector = NULL;
		free(rom_is_encrypted);
		rom_is_encrypted = NULL;
		free(can_decrypt_rom);
		can_decrypt_rom = NULL;
		free(successfully_decrypted_rom);
		successfully_decrypted_rom = NULL;
		free(rom_is_byte_swapped);
		rom_is_byte_swapped = NULL;
		free(rom_has_valid_checksum);
		rom_has_valid_checksum = NULL;
		free(rom_header_info);
		rom_header_info = NULL;
		free(rom_type);
		rom_type = NULL;
		free(rom_version);
		rom_version = NULL;
		free(embedded_rom_major_version);
		embedded_rom_major_version = NULL;
		free(embedded_rom_minor_version);
		embedded_rom_minor_version = NULL;
		free(detected_embedded_rom_version);
		detected_embedded_rom_version = NULL;
		free(is_kickety_split);
		is_kickety_split = NULL;
		return;
	}

	snprintf(has_valid_footer, 64, "ROM has valid footer:\t\t%d", amiga_rom->valid_footer);

	snprintf(output_string, string_length, "ROM Info:\n\n");
	snprintf(output_string, string_length, "%s%s\n", output_string, successfully_parsed);
	snprintf(output_string, string_length, "%s%s\n", output_string, rom_size_validated);
	snprintf(output_string, string_length, "%s%s\n", output_string, has_reset_vector);
	snprintf(output_string, string_length, "%s%s\n", output_string, rom_is_encrypted);
	snprintf(output_string, string_length, "%s%s\n", output_string, can_decrypt_rom);
	snprintf(output_string, string_length, "%s%s\n", output_string, successfully_decrypted_rom);
	snprintf(output_string, string_length, "%s%s\n", output_string, rom_is_byte_swapped);
	snprintf(output_string, string_length, "%s%s\n", output_string, rom_has_valid_checksum);
	snprintf(output_string, string_length, "%s%s\n", output_string, rom_header_info);
	snprintf(output_string, string_length, "%s%s\n", output_string, rom_type);
	snprintf(output_string, string_length, "%s%s\n", output_string, rom_version);
	snprintf(output_string, string_length, "%s%s\n", output_string, embedded_rom_major_version);
	snprintf(output_string, string_length, "%s%s\n", output_string, embedded_rom_minor_version);
	snprintf(output_string, string_length, "%s%s\n", output_string, detected_embedded_rom_version);
	snprintf(output_string, string_length, "%s%s\n", output_string, is_kickety_split);
	snprintf(output_string, string_length, "%s%s\n", output_string, has_valid_footer);

	free(successfully_parsed);
	successfully_parsed = NULL;
	free(rom_size_validated);
	rom_size_validated = NULL;
	free(has_reset_vector);
	has_reset_vector = NULL;
	free(rom_is_encrypted);
	rom_is_encrypted = NULL;
	free(can_decrypt_rom);
	can_decrypt_rom = NULL;
	free(successfully_decrypted_rom);
	successfully_decrypted_rom = NULL;
	free(rom_is_byte_swapped);
	rom_is_byte_swapped = NULL;
	free(rom_has_valid_checksum);
	rom_has_valid_checksum = NULL;
	free(rom_header_info);
	rom_header_info = NULL;
	free(rom_type);
	rom_type = NULL;
	free(rom_version);
	rom_version = NULL;
	free(embedded_rom_major_version);
	embedded_rom_major_version = NULL;
	free(embedded_rom_minor_version);
	embedded_rom_minor_version = NULL;
	free(detected_embedded_rom_version);
	detected_embedded_rom_version = NULL;
	free(is_kickety_split);
	is_kickety_split = NULL;
	free(has_valid_footer);
	has_valid_footer = NULL;
}

// Parses and validates the data in the Amiga ROM updates the struct
// passed in with that data.
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
	amiga_rom->is_byte_swapped = false;
	amiga_rom->has_valid_checksum = false;
	amiga_rom->header = 0;
	amiga_rom->type = 'U';
	amiga_rom->version = NULL;
	amiga_rom->major_version = 0;
	amiga_rom->minor_version = 0xffff;
	amiga_rom->major_minor_version = NULL;
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
			amiga_rom->is_encrypted = false;
			amiga_rom->can_decrypt = false;
			amiga_rom->successfully_decrypted = true;
		}
	}

	if(!amiga_rom->is_encrypted)
	{
		amiga_rom->parsed_rom = true;
		amiga_rom->validated_size = ValidateEmbeddedAmigaROMSize(amiga_rom);
		amiga_rom->has_reset_vector = ValidateAmigaROMResetVector(amiga_rom);
		amiga_rom->is_byte_swapped = (DetectAmigaROMByteSwap(amiga_rom) == 1);
		amiga_rom->has_valid_checksum = ValidateAmigaROMChecksum(amiga_rom);
		amiga_rom->header = DetectAmigaKickstartROMTypeFromHeader(amiga_rom);
		amiga_rom->type = DetectAmigaROMType(amiga_rom);
		amiga_rom->version = DetectAmigaROMVersion(amiga_rom);
		amiga_rom->major_version = DetectAmigaMajorROMVersion(amiga_rom);
		amiga_rom->minor_version = DetectAmigaMinorROMVersion(amiga_rom);
		amiga_rom->major_minor_version = DetectAmigaMajorMinorROMVersion(amiga_rom);
		amiga_rom->is_kickety_split = DetectKicketySplitAmigaROM(amiga_rom);
		amiga_rom->valid_footer = ValidateAmigaKickstartROMFooter(amiga_rom);
	}
}

// Return whether an Amiga ROM is a valid size, accounting for
// encryption, if it's there.
bool ValidateAmigaROMSize(const ParsedAmigaROMData *amiga_rom)
{
	size_t actual_rom_size = DetectUnencryptedAmigaROMSize(amiga_rom);

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

	return (be16toh(rom_data_16[104]) == 0x4E70);
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

	digest = (uint8_t*)malloc(20);
	if(!digest)
	{
		return NULL;
	}

	hexdigest = (char*)malloc(41);
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

// Detects the major version in the Amiga ROM header
// Returns 0 if the ROM is not detects as an Amiga ROM
uint16_t DetectAmigaMajorROMVersion(const ParsedAmigaROMData *amiga_rom)
{
	const uint16_t *rom_data_16;

	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size < 16)
	{
		return 0xffff;
	}

	if(!IsAmigaROM(amiga_rom))
	{
		return 0xffff;
	}

	rom_data_16 = (const uint16_t*)(amiga_rom->rom_data);

	return be16toh(rom_data_16[6]);
}

// Detects the minor version in the Amiga ROM header
// Returns 0 if the ROM is not detects as an Amiga ROM
uint16_t DetectAmigaMinorROMVersion(const ParsedAmigaROMData *amiga_rom)
{
	const uint16_t *rom_data_16;

	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size < 16)
	{
		return 0xffff;
	}

	if(!IsAmigaROM(amiga_rom))
	{
		return 0xffff;
	}

	rom_data_16 = (const uint16_t*)(amiga_rom->rom_data);

	return be16toh(rom_data_16[7]);
}

// Returns a version string based on the major and minor versions in the Amiga ROM header
// Returns NULL if the ROM is not detects as an Amiga ROM or the version is unknown
const char* DetectAmigaMajorMinorROMVersion(const ParsedAmigaROMData *amiga_rom)
{
	size_t i, number_of_versions;

	number_of_versions = sizeof(AMIGA_ROM_MAJOR_MINOR_VERSIONS) / sizeof(AmigaROMMajorMinorVersions);

	for(i = 0; i < number_of_versions; i++)
	{
		if(AMIGA_ROM_MAJOR_MINOR_VERSIONS[i].major_version == DetectAmigaMajorROMVersion(amiga_rom))
		{
			if((AMIGA_ROM_MAJOR_MINOR_VERSIONS[i].minor_version == 0xffff) || (AMIGA_ROM_MAJOR_MINOR_VERSIONS[i].minor_version == DetectAmigaMinorROMVersion(amiga_rom)))
			{
				return AMIGA_ROM_MAJOR_MINOR_VERSIONS[i].version;
			}
		}
	}

	return NULL;
}

// Returns a character indicating the type of ROM detected based on the SHA1 hash
// Valid return values are:
// A - Kickstart Hi/U34 ROM
// B - Kickstart Lo/U35 ROM
// E - Extended Amiga ROM
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

	digest = (uint8_t*)malloc(20);
	if(!digest)
	{
		return 'U';
	}

	hexdigest = (char*)malloc(41);
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

	return ((be32toh(rom_data_32[131072]) == AMIGA_256_ROM_HEADER) || (be32toh(rom_data_32[131072]) == AMIGA_256_ROM_HEADER_BYTESWAP));
}

// Detect which type of kickstart ROM a purported Kickstart ROM claims to be
// based on its header and size.  If this data is not consistent, or is unknown,
// the method returns 0x00.  Otherwise, the lower seven bits will determine what
// type of Kickstart ROM this is, according to the following table:
//
// 0x00: Not an Amiga ROM
// 0x01: 256KB ROM
// 0x02: 512KB ROM
// 0x03: Extended ROM
// 0x04: "ReKick" ROM
// 0x05: Ambiguous (may or may not be an Amiga ROM of another type)
//
// If the ROM is byte-swapped, then the topmost bit will be a 1, with the rest
// of the seven bits as the table above.
uint8_t DetectAmigaKickstartROMTypeFromHeader(const ParsedAmigaROMData *amiga_rom)
{
	const uint32_t *rom_data_32;

	uint32_t amiga_256_header = AMIGA_256_ROM_HEADER;
	uint32_t amiga_256_header_byteswap = AMIGA_256_ROM_HEADER_BYTESWAP;
	uint32_t amiga_512_header = AMIGA_512_ROM_HEADER;
	uint32_t amiga_512_header_byteswap = AMIGA_512_ROM_HEADER_BYTESWAP;
	uint32_t amiga_ext_header = AMIGA_EXT_ROM_HEADER;
	uint32_t amiga_ext_header_byteswap = AMIGA_EXT_ROM_HEADER_BYTESWAP;
	uint32_t amiga_rekick_rom_header = AMIGA_512_REKICK_ROM_HEADER;
	uint32_t amiga_rekick_rom_header_byteswap = AMIGA_512_REKICK_ROM_HEADER_BYTESWAP;
	uint32_t rom_header;

	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size == 0)
	{
		return 0x00;
	}

	rom_data_32 = (const uint32_t*)(amiga_rom->rom_data);

	if(amiga_rom->rom_size < 4)
	{
		return 0x00;
	}

	rom_header = be32toh(rom_data_32[0]);

	if(amiga_rom->rom_size == 524288)
	{
		if((rom_header == amiga_512_header) || (rom_header == amiga_512_header_byteswap))
		{
			if(rom_header == amiga_512_header)
			{
				return 0x02;
			}
			else
			{
				return 0x02 | 0x80;
			}
		}
	}
	else if(amiga_rom->rom_size == 262144)
	{
		if((rom_header == amiga_256_header) || (rom_header == amiga_256_header_byteswap))
		{
			if(rom_header == amiga_256_header)
			{
				return 0x01;
			}
			else
			{
				return 0x01 | 0x80;
			}
		}
		else if((rom_header == amiga_ext_header) || (rom_header == amiga_ext_header_byteswap))
		{
			if(rom_header == amiga_ext_header)
			{
				return 0x03;
			}
			else
			{
				return 0x03 | 0x80;
			}
		}
		else if((rom_header == amiga_rekick_rom_header) || (rom_header == amiga_rekick_rom_header_byteswap))
		{
			if(rom_header == amiga_rekick_rom_header)
			{
				return 0x04;
			}
			else
			{
				return 0x04 | 0x80;
			}
		}
	}
	else if(amiga_rom->rom_size == 8192 || amiga_rom->rom_size == 16384 || amiga_rom->rom_size == 32768 || amiga_rom->rom_size == 131072)
	{
		return 0x05;
	}

	return 0x00;
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

	temp_rom_data = (uint8_t*)malloc(amiga_rom->rom_size);
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
		result_buffer = (uint8_t*)malloc((amiga_rom->rom_size + 11));
	}
	else
	{
		result_buffer = (uint8_t*)malloc((amiga_rom->rom_size - 11));
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
		keyfile_size = (size_t)ftell(fp);
		fseek(fp, 0, SEEK_SET);
	}

	keyfile_buffer = (uint8_t*)malloc(keyfile_size);
	if(!keyfile_buffer)
	{
		free(result_buffer);
		result_buffer = NULL;
		return false;
	}

	fread(keyfile_buffer, 1, keyfile_size, fp);

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
		amiga_rom->rom_data = realloc(amiga_rom->rom_data, result_size);
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
		amiga_rom->rom_data = realloc(amiga_rom->rom_data, (result_size + 11));
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
// -1 indicates the ROM is not an Amiga ROM known to this library,
//    and the header is not indicative of its byte swappiness.
int DetectAmigaROMByteSwap(const ParsedAmigaROMData *amiga_rom)
{
	uint8_t *digest;
	char *hexdigest;

	uint8_t rom_type = 0;
	size_t i;
	size_t rom_quantity = sizeof(AMIGA_ROM_INFO) / sizeof(AmigaROMInfo);

	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size == 0)
	{
		return -1;
	}

	digest = (uint8_t*)malloc(20);
	if(!digest)
	{
		return -1;
	}

	hexdigest = (char*)malloc(41);
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

	rom_type = DetectAmigaKickstartROMTypeFromHeader(amiga_rom);

	if((rom_type & 0x80) == 0x80)
	{
		return 1;
	}
	else if(rom_type != 0)
	{
		return 0;
	}

	return -1;
}

// Get the unencrypted size of the Amiga ROM in bytes, based on the
// data passed in.
size_t DetectUnencryptedAmigaROMSize(const ParsedAmigaROMData *amiga_rom)
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

	return actual_rom_size;
}

// If swap_unconditionally is true, the method will swap the ROM's bytes regardless of whether
// or not it is a known ROM.
// Returns true for success or false for failure.
bool SetAmigaROMByteSwap(ParsedAmigaROMData *amiga_rom, const bool swap_bytes, const bool unswap_bytes, const bool swap_unconditionally)
{
	size_t i;
	int is_swapped = 0;

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
bool SplitAmigaROM(const ParsedAmigaROMData *amiga_rom, ParsedAmigaROMData *rom_high, ParsedAmigaROMData *rom_low)
{
	uint8_t *test_ptr;

	size_t i;

	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size == 0 || amiga_rom->rom_size % 2 != 0 || !rom_high || !rom_low)
	{
		return false;
	}

	ParsedAmigaROMData *temp_rom_data = (ParsedAmigaROMData*)malloc(sizeof(*amiga_rom));

	memcpy(temp_rom_data, amiga_rom, sizeof(*amiga_rom));

	if(temp_rom_data->parsed_rom)
	{
		SetAmigaROMByteSwap(temp_rom_data, !(temp_rom_data->is_byte_swapped), temp_rom_data->is_byte_swapped, false);
	}

	if(rom_high->rom_data)
	{
		test_ptr = realloc(rom_high->rom_data, amiga_rom->rom_size);
		printf("fdsa\n");
		if(!test_ptr)
		{
			free(temp_rom_data);
			return false;
		}
		else
		{
			rom_high->rom_data = test_ptr;
			rom_high->rom_size = amiga_rom->rom_size;
			test_ptr = NULL;
		}
	}
	else
	{
		test_ptr = (uint8_t*)malloc(amiga_rom->rom_size);
		if(!test_ptr)
		{
			free(temp_rom_data);
			return false;
		}
		else
		{
			rom_high->rom_data = test_ptr;
			rom_high->rom_size = amiga_rom->rom_size;
			test_ptr = NULL;
		}
	}

	if(rom_low->rom_data)
	{
		printf("dddd\n");
		printf("Low ROM: %p\n", (void*)rom_low);
		printf("Low ROM data: %p\n", (void*)rom_low->rom_data);
		test_ptr = realloc(rom_low->rom_data, amiga_rom->rom_size);
		printf("asdf\n");
		if(!test_ptr)
		{
			free(temp_rom_data);
			return false;
		}
		else
		{
			rom_low->rom_data = test_ptr;
			rom_low->rom_size = amiga_rom->rom_size;
			test_ptr = NULL;
		}
	}
	else
	{
		test_ptr = (uint8_t*)malloc(amiga_rom->rom_size);
		if(!test_ptr)
		{
			free(temp_rom_data);
			return false;
		}
		else
		{
			rom_low->rom_data = test_ptr;
			rom_low->rom_size = amiga_rom->rom_size;
			test_ptr = NULL;
		}
	}

	for(i = 0; i < amiga_rom->rom_size; i = i + 4)
	{
		memcpy(&(rom_high->rom_data)[i / 2], &(amiga_rom->rom_data)[i], 2);
		memcpy(&(rom_low->rom_data)[i / 2], &(amiga_rom->rom_data)[i + 2], 2);
	}

	memcpy(&(rom_high->rom_data)[amiga_rom->rom_size / 2], &(rom_high->rom_data)[0], amiga_rom->rom_size / 2);
	memcpy(&(rom_low->rom_data)[amiga_rom->rom_size / 2], &(rom_low->rom_data)[0], amiga_rom->rom_size / 2);

	ParseAmigaROMData(rom_high, NULL);
	ParseAmigaROMData(rom_low, NULL);

	free(temp_rom_data);

	return true;
}

// For this method, ROM A and ROM B should each be the same size as the merged ROM.
// Each A and B ROM gets the same contents repeated twice.
// Returns true if it succeeds, or false if it doesn't.
bool MergeAmigaROM(const ParsedAmigaROMData *rom_high, const ParsedAmigaROMData *rom_low, ParsedAmigaROMData *amiga_rom)
{
	uint8_t *test_ptr;

	size_t i;

	if(!amiga_rom || !rom_high || !(rom_high->rom_data) || !rom_low || !(rom_low->rom_data) || rom_high->rom_size != rom_low->rom_size || rom_high->rom_size == 0 || rom_high->rom_size % 4 != 0)
	{
		return false;
	}

	if(amiga_rom->rom_data)
	{
		test_ptr = realloc(amiga_rom->rom_data, rom_high->rom_size);
		if(!test_ptr)
		{
			return false;
		}
		else
		{
			amiga_rom->rom_data = test_ptr;
			amiga_rom->rom_size = rom_high->rom_size;
		}
	}
	else
	{
		test_ptr = (uint8_t*)malloc(rom_high->rom_size);
		if(!test_ptr)
		{
			return false;
		}
		else
		{
			amiga_rom->rom_data = test_ptr;
			amiga_rom->rom_size = rom_high->rom_size;
			test_ptr = NULL;
		}
	}

	for(i = 0; i < rom_high->rom_size / 4; i++)
	{
		memcpy(&(amiga_rom->rom_data)[i * 4], &(rom_high->rom_data)[i * 2], 2);
		memcpy(&(amiga_rom->rom_data)[(i * 4) + 2], &(rom_low->rom_data)[i * 2], 2);
	}

	amiga_rom->rom_size = rom_high->rom_size;

	ParseAmigaROMData(amiga_rom, NULL);

	return true;
}

// Write a ROM to disk and return a bool indicating whether the write
// was successful or not.
bool WriteAmigaROM(const ParsedAmigaROMData *amiga_rom, const char *rom_file_path)
{
	FILE *fp;

	size_t bytes_written = 0;

	if(!amiga_rom || !(amiga_rom->rom_data) || amiga_rom->rom_size == 0 || !rom_file_path)
	{
		return false;
	}

	fp = fopen(rom_file_path, "wb");
	if(!fp)
	{
		return false;
	}

	bytes_written = fwrite(amiga_rom->rom_data, 1, amiga_rom->rom_size, fp);
	fclose(fp);

	if(bytes_written != amiga_rom->rom_size)
	{
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4996)
#endif
		unlink(rom_file_path);
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
		return false;
	}

	return true;
}
