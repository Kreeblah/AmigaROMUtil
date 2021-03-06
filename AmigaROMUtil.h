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

#ifndef AMIGAROMUTIL_H
#define AMIGAROMUTIL_H

#include "AmigaROMHashes.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	bool is_initialized;
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
	uint8_t header;
	char type;
	const char *version;
	uint16_t major_version;
	uint16_t minor_version;
	const char *major_minor_version;
	bool is_kickety_split;
	bool valid_footer;
} ParsedAmigaROMData;

// Create and return a new and initialized struct.
// Pointers are NOT allocated, but are NULL instead.
ParsedAmigaROMData GetInitializedAmigaROM(void);

// Free all pointers which are expected to potentially be
// allocated in a struct and sets the pointers to NULL,
// and sets the rest of the struct to default values.
void DestroyInitializedAmigaROM(ParsedAmigaROMData *amiga_rom);

// Returns a parsed ROM data struct, with the ROM data and size included.
// If anything files, parsed_rom will be false.  If encrypted, the ROM
// will be decrypted.
ParsedAmigaROMData ReadAmigaROM(const char *rom_file_path, const char *keyfile_path);

// Detect whether a ROM is an Amiga kickstart ROM based on size, header, reset vector,
// magic, and footer.
bool IsAmigaROM(const ParsedAmigaROMData *amiga_rom);

// Puts ROM info data into output_string
void PrintAmigaROMInfo(const ParsedAmigaROMData *amiga_rom, char *output_string, const size_t string_length);

// Parses and validates the data in the Amiga ROM updates the struct
// passed in with that data.
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

// Detects the major version in the Amiga ROM header
// Returns 0 if the ROM is not detects as an Amiga ROM
uint16_t DetectAmigaMajorROMVersion(const ParsedAmigaROMData *amiga_rom);

// Detects the minor version in the Amiga ROM header
// Returns 0 if the ROM is not detects as an Amiga ROM
uint16_t DetectAmigaMinorROMVersion(const ParsedAmigaROMData *amiga_rom);

// Returns a version string based on the major and minor versions in the Amiga ROM header
// Returns NULL if the ROM is not detects as an Amiga ROM or the version is unknown
const char* DetectAmigaMajorMinorROMVersion(const ParsedAmigaROMData *amiga_rom);

// Returns a character indicating the type of ROM detected based on the SHA1 hash
// Valid return values are:
// A - Kickstart Hi/U34 ROM
// B - Kickstart Lo/U35 ROM
// E - Extended Amiga ROM
// M - Kickstart merged ROM
// O - Other (non-Kickstart) ROM
// U - Unknown ROM
char DetectAmigaROMType(const ParsedAmigaROMData *amiga_rom);

// Detect whether a ROM is a "Kickety-Split ROM"
// Returns true if it is, and false if it isn't.
bool DetectKicketySplitAmigaROM(const ParsedAmigaROMData *amiga_rom);

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
uint8_t DetectAmigaKickstartROMTypeFromHeader(const ParsedAmigaROMData *amiga_rom);

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
bool DoAmigaROMCryptOperation(uint8_t *rom_data_without_crypt_header, const size_t rom_size, const uint8_t *keyfile_data, const size_t keyfile_size);

// 0 indicates the ROM is not byte swapped (the ROM is for emulators)
// 1 indicates the ROM is byte swapped (the ROM is for physical ICs)
// -1 indicates the ROM is not an Amiga ROM known to this library,
//    and the header is not indicative of its byte swappiness.
int DetectAmigaROMByteSwap(const ParsedAmigaROMData *amiga_rom);

// Get the unencrypted size of the Amiga ROM in bytes, based on the
// data passed in.
size_t DetectUnencryptedAmigaROMSize(const ParsedAmigaROMData *amiga_rom);

// If swap_unconditionally is true, the method will swap the ROM's bytes regardless of whether
// or not it is a known ROM.
// Returns true for success or false for failure.
bool SetAmigaROMByteSwap(ParsedAmigaROMData *amiga_rom, const bool swap_bytes, const bool unswap_bytes, const bool swap_unconditionally);

// For this method, ROM A and ROM B should each be the same size as the merged ROM.
// Each A and B ROM gets the same contents repeated twice.
bool SplitAmigaROM(const ParsedAmigaROMData *amiga_rom, ParsedAmigaROMData *rom_high, ParsedAmigaROMData *rom_low);

// For this method, ROM A and ROM B should each be the same size as the merged ROM.
// Each A and B ROM gets the same contents repeated twice.
bool MergeAmigaROM(const ParsedAmigaROMData *rom_high, const ParsedAmigaROMData *rom_low, ParsedAmigaROMData *amiga_rom);

// Write a ROM to disk and return a bool indicating whether the write
// was successful or not.
bool WriteAmigaROM(const ParsedAmigaROMData *amiga_rom, const char *rom_file_path);

#endif
