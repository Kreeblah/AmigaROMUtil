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

#include "AmigaROMHashes.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	bool parsed_rom;
	uint8_t *rom_data;
	size_t rom_size;
	bool validated_size;
	bool has_reset_vector;
	bool is_encrypted;
	bool can_decrypt;
	bool successfully_decrypted;
	bool is_byte_swapped;
	bool has_valid_checksum;
	char header;
	char type;
	const char *version;
	bool is_kickety_split;
	bool valid_footer;
} ParsedAmigaROMData;

// Returns a parsed ROM data struct, with the ROM data and size included.
// If anything files, parsed_rom will be false.  If encrypted, the ROM
// will be decrypted.
ParsedAmigaROMData ReadAmigaROM(const char *rom_file_path, const char *keyfile_path);

// Puts ROM info data into output_string
void PrintAmigaROMInfo(const ParsedAmigaROMData *amiga_rom, char *output_string, const size_t string_length);

// Parses and validates the data in the Amiga ROM and returns what is found
// in a ParsedAmigaROMData struct.
void ParseAmigaROMData(ParsedAmigaROMData *amiga_rom, const char* keyfile_path);

// Return whether an Amiga ROM is a valid size, accounting for
// encryption, if it's there.
bool ValidateAmigaROMSize(const ParsedAmigaROMData *amiga_rom);

// Validate that there is a reset vector (0x0x4E70) at 0x000000D0.
// Returns true if there is, or false if there isn't.
bool ValidateAmigaROMResetVector(const ParsedAmigaROMData *amiga_rom);

// Detects the version of the ROM by SHA1 hash
// Returns NULL for failure, else a string indicating the ROM version
const char* DetectAmigaROMVersion(const ParsedAmigaROMData *amiga_rom);

// Returns a character indicating the type of ROM detected based on the SHA1 hash
// Valid return values are:
// A - Kickstart Hi/U34 ROM
// B - Kickstart Lo/U35 ROM
// M - Kickstart merged ROM
// O - Other (non-Kickstart) ROM
// U - Unknown ROM
char DetectAmigaROMType(const ParsedAmigaROMData *amiga_rom);

// Detect whether a ROM is a "Kickety-Split ROM"
// Returns true if it is, and false if it isn't.
bool DetectKicketySplitAmigaROM(const ParsedAmigaROMData *amiga_rom);

// Detect which type of kickstart ROM a purported Kickstart ROM claims to be
// based on its header and size.  If this data is not consistent, or is unknown,
// the method returns 'U'.  Otherwise, it returns '5' for a 512k ROM, '2' for a
// 256k ROM, 'E' for an extended ROM, or 'R' for a "ReKick" ROM.
char DetectAmigaKickstartROMTypeFromHeader(const ParsedAmigaROMData *amiga_rom);

// Returns an unsigned 32-bit integer with the checksum for the ROM.
// This checksum is used by Amigas to validate whether a ROM is undamaged,
// and is located 24 bytes from the end of the ROM.
// If calc_new_sum is true, then the function will return a checksum value which
// will be accepted for use by an Amiga system.
uint32_t CalculateAmigaROMChecksum(const ParsedAmigaROMData *amiga_rom, const bool calc_new_sum);

// Returns the checksum embedded in the ROM, or 0 if it fails.
// Technically a ROM could have a valid checksum of zero, but 
// this is exceedingly unlikely.
uint32_t GetEmbeddedAmigaROMChecksum(const ParsedAmigaROMData *amiga_rom);

// Returns a boolean indicating whether the calculated checksum in the ROM
// matches the calculated checksum for the ROM.
bool ValidateAmigaROMChecksum(const ParsedAmigaROMData *amiga_rom);

// Calculates and embeds a correct checksum in the ROM.
// Returns true if it succeeds, or false if it fails.
bool CorrectAmigaROMChecksum(ParsedAmigaROMData *amiga_rom);

// Validates whether an Amiga kickstart ROM as a valid footer.
// Returns true if it does, or false if it doesn't.
bool ValidateAmigaKickstartROMFooter(const ParsedAmigaROMData *amiga_rom);

// Validate the ROM size matches the size embedded in the ROM.
// Returns true if it does, or false if it doesn't.
bool ValidateEmbeddedAmigaROMSize(const ParsedAmigaROMData *amiga_rom);

// Detects whether an Amiga ROM is encrypted.
// Returns 1 if it is, 0 if it isn't, or -1 if it has an invalid size.
int DetectAmigaROMEncryption(const ParsedAmigaROMData *amiga_rom);

// Encrypts or decrypts Amiga ROMs according to whether crypt_operation is true or false.
// If true, it will encrypt the ROMs.  If false, it will decrypt them.
// The function returns true if the method succeeds, and false if it fails.
// For safety, make sure that rom_contents has space for 11 more bytes than the original
// ROM had if an encrypt operation is performed.
bool CryptAmigaROM(ParsedAmigaROMData *amiga_rom, const bool crypt_operation, const char *keyfile_path);

// Run the actual crypt operation, using the ROM data and keyfile data
void DoAmigaROMCryptOperation(uint8_t *rom_data_without_crypt_header, const size_t rom_size, const uint8_t *keyfile_data, const size_t keyfile_size);

// 0 indicates the ROM is not byte swapped (the ROM is for emulators)
// 1 indicates the ROM is byte swapped (the ROM is for physical ICs)
// -1 indicates the ROM is not an Amiga ROM known to this library
int DetectAmigaROMByteSwap(const ParsedAmigaROMData *amiga_rom);

// If swap_unconditionally is true, the method will swap the ROM's bytes regardless of whether
// or not it is a known ROM.
// Returns true for success or false for failure.
bool SetAmigaROMByteSwap(ParsedAmigaROMData *amiga_rom, const bool swap_bytes, const bool unswap_bytes, const bool swap_unconditionally);

// For this method, ROM A and ROM B should each be the same size as the merged ROM.
// Each A and B ROM gets the same contents repeated twice.
bool SplitAmigaROM(const ParsedAmigaROMData *amiga_rom, uint8_t *rom_high_contents, uint8_t *rom_low_contents);

// For this method, ROM A and ROM B should each be the same size as the merged ROM.
// Each A and B ROM gets the same contents repeated twice.
bool MergeAmigaROM(const uint8_t *rom_high_contents, const uint8_t *rom_low_contents, const size_t split_rom_size, ParsedAmigaROMData *amiga_rom);

#endif