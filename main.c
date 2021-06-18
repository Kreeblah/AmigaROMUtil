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

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_help();
int split_rom(const bool swap, const bool unswap, const bool unconditional_swap, const char* encryption_key_path, const bool correct_checksum, const char* rom_a_path, const char* rom_b_path, const char* rom_input_path);
int merge_rom(const bool swap, const bool unswap, const bool unconditional_swap, const bool encrypt_rom, const char* encryption_key_path, const bool correct_checksum, const char* rom_a_path, const char* rom_b_path, const char* rom_output_path);
int swap_rom(const bool swap_state, const bool unconditional_swap, const bool encrypt_rom, const char* encryption_key_path, const bool correct_checksum, const char* rom_input_path, const char* rom_output_path);
int crypt_rom(const bool encryption_state, const char* encryption_key_path, const char* rom_input_path, const char* rom_output_path);
int checksum_rom(const bool correct_checksum, const char* encryption_key_path, const char* rom_input_path, const char* rom_output_path);

int main(int argc, char** argv)
{
	char* rom_input_path = NULL;
	char* rom_output_path = NULL;
	char* rom_a_path = NULL;
	char* rom_b_path = NULL;
	char* encryption_key_path = NULL;
	bool split = false;
	bool merge = false;
	bool swap = false;
	bool unswap = false;
	bool unconditional_swap = false;
	bool validate_checksum = false;
	bool correct_checksum = false;
	bool encrypt_rom = false;
	bool decrypt_rom = false;
	int c;
	int operation_result = 0;

	while((c = getopt(argc, argv, "i:o:a:b:k:sgpunvcedh")) != -1)
	{
		switch(c)
		{
			case 'i':
				rom_input_path = strdup(optarg);
				break;
			case 'o':
				rom_output_path = strdup(optarg);
				break;
			case 'a':
				rom_a_path = strdup(optarg);
				break;
			case 'b':
				rom_b_path = strdup(optarg);
				break;
			case 'k':
				encryption_key_path = strdup(optarg);
				break;
			case 's':
				split = true;
				break;
			case 'g':
				merge = true;
				break;
			case 'p':
				swap = true;
				break;
			case 'u':
				unswap = true;
				break;
			case 'n':
				unconditional_swap = true;
				break;
			case 'v':
				validate_checksum = true;
				break;
			case 'c':
				correct_checksum = true;
				break;
			case 'e':
				encrypt_rom = true;
				break;
			case 'd':
				decrypt_rom = true;
				break;
			case 'h':
			default:
				print_help();
				exit(0);				
		}
	}

	if(unconditional_swap)
	{
		swap = true;
		unswap = false;
	}

	if(!split && !merge && !swap && !unswap && !encrypt_rom && !decrypt_rom && !validate_checksum && !correct_checksum)
	{
		print_help();
		exit(1);
	}

	if(split && merge)
	{
		print_help();
		exit(1);
	}

	if(encrypt_rom && decrypt_rom)
	{
		print_help();
		exit(1);
	}

	if(split && (!rom_input_path || !rom_a_path || !rom_b_path))
	{
		print_help();
		exit(1);
	}

	if(merge && (!rom_output_path || !rom_a_path || !rom_b_path))
	{
		print_help();
		exit(1);
	}

	if((swap || unswap || encrypt_rom || decrypt_rom || correct_checksum) && (!rom_input_path || !rom_output_path))
	{
		print_help();
		exit(1);
	}

	if(validate_checksum && (!rom_input_path && !merge))
	{
		print_help();
		exit(1);
	}

	if((encrypt_rom || decrypt_rom) && !encryption_key_path)
	{
		print_help();
		exit(1);
	}

	if(split)
	{
		operation_result = split_rom(swap, unswap, unconditional_swap, encryption_key_path, correct_checksum, rom_a_path, rom_b_path, rom_input_path);
	}
	else if(merge)
	{
		operation_result = merge_rom(swap, unswap, unconditional_swap, encrypt_rom, encryption_key_path, correct_checksum, rom_a_path, rom_b_path, rom_output_path);
	}
	else if(swap)
	{
		operation_result = swap_rom(true, unconditional_swap, encrypt_rom, encryption_key_path, correct_checksum, rom_input_path, rom_output_path);
	}
	else if(unswap)
	{
		operation_result = swap_rom(false, unconditional_swap, encrypt_rom, encryption_key_path, correct_checksum, rom_input_path, rom_output_path);
	}
	else if(encrypt_rom)
	{
		operation_result = crypt_rom(true, encryption_key_path, rom_input_path, rom_output_path);
	}
	else if(decrypt_rom)
	{
		operation_result = crypt_rom(false, encryption_key_path, rom_input_path, rom_output_path);
	}
	else if(correct_checksum)
	{
		operation_result = checksum_rom(true, encryption_key_path, rom_input_path, rom_output_path);
	}
	else if(validate_checksum)
	{
		operation_result = checksum_rom(false, encryption_key_path, rom_input_path, NULL);
	}

	free(rom_a_path);
	free(rom_b_path);
	free(rom_input_path);
	free(rom_output_path);
	free(encryption_key_path);

	exit(operation_result);
}

void print_help()
{
    printf("Usage: AmigaROMUtil [options]\n");
    printf("Options:\n");
    printf("  -i FILE  Path to input ROM (except for merging)\n");
    printf("  -o FILE  Path to output ROM (except for splitting)\n");
    printf("  -a FILE  Path to A ROM for merging or splitting\n");
    printf("  -b FILE  Path to B ROM for merging or splitting\n");
    printf("  -k FILE  Path to ROM encryption/decryption key\n");
    printf("  -s       Split ROM (requires -i, -a, -b)\n");
    printf("  -g       Merge ROM (requires -a, -b, -o)\n");
    printf("  -p       Byte swap ROM for burning to an IC (requires -i, -o)\n");
    printf("  -u       Unbyte swap ROM for use with emulators (requires -i, -o)\n");
    printf("  -n       Unconditional byte swap for ROMs which are unknown to this program\n");
    printf("  -v       Validate checksum (requires -i)\n");
    printf("  -c       Correct checksum (requires -i, -o)\n");
    printf("  -e       Encrypt ROM (requires -i, -o, -k)\n");
    printf("  -d       Decrypt ROM (requires -i, -o, -k)\n");
    printf("  -h,      Display this information\n");
    printf("\n");
    printf("Notes:\n");
    printf("-n implies -u and not -n, and will override those if set\n");
    printf("-s and -m, -p and -u, -e and -d are each mutually exclusive\n");
	return;
}

int split_rom(const bool swap, const bool unswap, const bool unconditional_swap, const char* encryption_key_path, const bool correct_checksum, const char* rom_a_path, const char* rom_b_path, const char* rom_input_path)
{
	FILE *fp;
	size_t bytes_written = 0;
	uint8_t rom_a_buffer[MAX_AMIGA_ROM_SIZE], rom_b_buffer[MAX_AMIGA_ROM_SIZE], input_rom_buffer[MAX_AMIGA_ROM_SIZE];
	size_t input_rom_size = 0;
	size_t decrypted_input_rom_size = 0;
	const char* input_rom_version;
	char input_rom_type = '\0';

	input_rom_size = ReadAmigaROM(rom_input_path, input_rom_buffer);
	if(input_rom_size == 0)
	{
		printf("ERROR: Unable to open file: %s\n", rom_input_path);
		printf("Is it a valid ROM?\n");
		return 1;
	}

	if(DetectAmigaROMEncryption(input_rom_buffer))
	{
		if(encryption_key_path == NULL)
		{
			printf("ERROR: ROM is encrypted.  Specify a decryption key to decrypt.\n");
			return 1;
		}
		else
		{
			decrypted_input_rom_size = CryptAmigaROM(input_rom_buffer, input_rom_size, false, encryption_key_path);

			if(decrypted_input_rom_size == input_rom_size)
			{
				printf("ERROR: Unable to access ROM decryption key.\n");
				return 1;
			}
			else
			{
				input_rom_size = decrypted_input_rom_size;
			}
		}
	}

	input_rom_version = DetectAmigaROMVersion(input_rom_buffer, input_rom_size);
	if(input_rom_version == NULL)
	{
		printf("WARNING: Unknown ROM loaded.\n");
	}
	else
	{
		printf("Detected ROM: %s\n", input_rom_version);
	}

	input_rom_type = DetectAmigaROMType(input_rom_buffer, input_rom_size);
	if(input_rom_type != 'M')
	{
		printf("ROM type: %c\n", input_rom_type);
		printf("WARNING: ROM is not detected as a known merged ROM.\n");
	}

	if((input_rom_type != 'U' || unconditional_swap) && SetAmigaROMByteSwap(input_rom_buffer, input_rom_size, swap, unswap, unconditional_swap) == 0)
	{
		printf("ERROR: Unable to perform conditional swap operation.  Aborting.\n");
		return 1;
	}

	if(ValidateAmigaROMChecksum(input_rom_buffer, input_rom_size))
	{
		printf("Source ROM checksum is valid.\n");
	}
	else
	{
		if(correct_checksum)
		{
			if(CorrectAmigaROMChecksum(input_rom_buffer, input_rom_size))
			{
				printf("Corrected ROM checksum.\n");
			}
			else
			{
				printf("ERROR: Unable to correct ROM checksum.\n");
				return 1;
			}
		}
		else
		{
			printf("WARNING: Source ROM checksum is invalid.\n");
		}
	}

	SplitAmigaROM(input_rom_buffer, input_rom_size, rom_a_buffer, rom_b_buffer);

	fp = fopen(rom_a_path, "wb");
	if(!fp)
	{
		printf("ERROR: Unable to open file for writing A ROM: %s\n", rom_a_path);
		return 1;
	}

	bytes_written = fwrite(rom_a_buffer, sizeof(uint8_t), input_rom_size, fp);
	if(bytes_written != input_rom_size)
	{
		printf("ERROR: Wrote %zu bytes when expected to write %zu bytes for ROM A.  Aborting.\n", bytes_written, input_rom_size);
		return 1;
	}

	fclose(fp);

	fp = fopen(rom_b_path, "wb");
	if(!fp)
	{
		printf("ERROR: Unable to open file for writing B ROM: %s\n", rom_b_path);
		return 1;
	}

	bytes_written = fwrite(rom_b_buffer, sizeof(uint8_t), input_rom_size, fp);
	fclose(fp);

	if(bytes_written != input_rom_size)
	{
		printf("ERROR: Wrote %zu bytes when expected to write %zu bytes for ROM B.  Aborting.\n", bytes_written, input_rom_size);
		return 1;
	}

	printf("Successfully wrote A and B ROMs.\n");

	return 0;
}

int merge_rom(const bool swap, const bool unswap, const bool unconditional_swap, const bool encrypt_rom, const char* encryption_key_path, const bool correct_checksum, const char* rom_a_path, const char* rom_b_path, const char* rom_output_path)
{
	FILE *fp;
	size_t bytes_written = 0;
	uint8_t rom_a_buffer[MAX_AMIGA_ROM_SIZE], rom_b_buffer[MAX_AMIGA_ROM_SIZE], output_rom_buffer[MAX_AMIGA_ROM_SIZE];
	size_t rom_a_size = 0;
	size_t rom_b_size = 0;
	size_t decrypted_a_rom_size = 0;
	size_t decrypted_b_rom_size = 0;
	size_t encrypted_output_rom_size = 0;
	char rom_a_type = '\0';
	char rom_b_type = '\0';
	const char* rom_a_version;
	const char* rom_b_version;

	rom_a_size = ReadAmigaROM(rom_a_path, rom_a_buffer);
	if(rom_a_size == 0)
	{
		printf("ERROR: Unable to open file: %s\n", rom_a_path);
		printf("Is it a valid ROM?\n");
		return 1;
	}

	rom_b_size = ReadAmigaROM(rom_b_path, rom_b_buffer);
	if(rom_b_size == 0)
	{
		printf("ERROR: Unable to open file: %s\n", rom_b_path);
		printf("Is it a valid ROM?\n");
		return 1;
	}

	if(DetectAmigaROMEncryption(rom_a_buffer))
	{
		if(encryption_key_path == NULL)
		{
			printf("ERROR: ROM A is encrypted.  Specify a decryption key to decrypt.\n");
			return 1;
		}
		else
		{
			decrypted_a_rom_size = CryptAmigaROM(rom_a_buffer, rom_a_size, false, encryption_key_path);

			if(decrypted_a_rom_size == rom_a_size)
			{
				printf("ERROR: Unable to access ROM decryption key.\n");
				return 1;
			}
			else
			{
				rom_a_size = decrypted_a_rom_size;
			}
		}
	}

	if(DetectAmigaROMEncryption(rom_b_buffer))
	{
		if(encryption_key_path == NULL)
		{
			printf("ERROR: ROM B is encrypted.  Specify a decryption key to decrypt.\n");
			return 1;
		}
		else
		{
			decrypted_b_rom_size = CryptAmigaROM(rom_b_buffer, rom_b_size, false, encryption_key_path);

			if(decrypted_b_rom_size == rom_b_size)
			{
				printf("ERROR: Unable to access ROM decryption key.\n");
				return 1;
			}
			else
			{
				rom_b_size = decrypted_b_rom_size;
			}
		}
	}

	if(rom_a_size != rom_b_size)
	{
		printf("ERROR: A and B ROMs must be the same size to merge.\n");
		return 1;
	}

	rom_a_version = DetectAmigaROMVersion(rom_a_buffer, rom_a_size);
	if(rom_a_version == NULL)
	{
		printf("WARNING: Unknown ROM loaded.\n");
	}
	else
	{
		printf("Detected ROM: %s\n", rom_a_version);
	}

	rom_b_version = DetectAmigaROMVersion(rom_b_buffer, rom_b_size);
	if(rom_b_version == NULL)
	{
		printf("WARNING: Unknown ROM loaded.\n");
	}
	else
	{
		printf("Detected ROM: %s\n", rom_b_version);
	}

	if(rom_a_version != NULL && rom_b_version == NULL)
	{
		printf("WARNING: Known A ROM and unknown B ROM detected.\n");
	}
	else if(rom_b_version != NULL && rom_a_version == NULL)
	{
		printf("WARNING: Known B ROM and unknown A ROM detected.\n");
	}
	else if((rom_a_version != NULL && rom_b_version != NULL) && (strlen(rom_a_version) != strlen(rom_b_version) || strncmp(rom_a_version, rom_b_version, strlen(rom_a_version)) != 0))
	{
		printf("WARNING: A and B ROMs are not from the same set.\n");
	}

	rom_a_type = DetectAmigaROMType(rom_a_buffer, rom_a_size);
	if(rom_a_type != 'A')
	{
		printf("WARNING: A rom is not detected as a known A ROM.\n");
	}

	rom_b_type = DetectAmigaROMType(rom_b_buffer, rom_b_size);
	if(rom_b_type != 'A')
	{
		printf("WARNING: B rom is not detected as a known B ROM.\n");
	}

	if((rom_a_type != 'U' || unconditional_swap) && SetAmigaROMByteSwap(rom_a_buffer, rom_a_size, swap, unswap, unconditional_swap) == 0)
	{
		printf("ERROR: Unable to perform conditional swap operation for ROM A.  Aborting.\n");
		return 1;
	}

	if((rom_b_type != 'U' || unconditional_swap) && SetAmigaROMByteSwap(rom_b_buffer, rom_b_size, swap, unswap, unconditional_swap) == 0)
	{
		printf("ERROR: Unable to perform conditional swap operation for ROM B.  Aborting.\n");
		return 1;
	}

	MergeAmigaROM(rom_a_buffer, rom_b_buffer, rom_a_size, output_rom_buffer);

	if(ValidateAmigaROMChecksum(output_rom_buffer, rom_a_size))
	{
		printf("Merged ROM checksum is valid.\n");
	}
	else
	{
		if(correct_checksum)
		{
			if(CorrectAmigaROMChecksum(output_rom_buffer, rom_a_size))
			{
				printf("Corrected ROM checksum.\n");
			}
			else
			{
				printf("ERROR: Unable to correct ROM checksum.\n");
				return 1;
			}
		}
		else
		{
			printf("WARNING: Source ROM checksum is invalid.\n");
		}
	}

	if(encrypt_rom)
	{
		if(encryption_key_path == NULL)
		{
			printf("ERROR: Encrypting a ROM requires specifying an encryption key.\n");
			return 1;
		}
		encrypted_output_rom_size = CryptAmigaROM(output_rom_buffer, rom_a_size, true, encryption_key_path);

		if(encrypted_output_rom_size == rom_a_size)
		{
			printf("ERROR: Unable to access ROM encryption key.\n");
			return 1;
		}
		else
		{
			rom_a_size = encrypted_output_rom_size;
			rom_b_size = rom_a_size;
		}
	}

	fp = fopen(rom_output_path, "wb");
	if(!fp)
	{
		printf("ERROR: Unable to open file for writing merged ROM: %s\n", rom_output_path);
		return 1;
	}

	bytes_written = fwrite(output_rom_buffer, sizeof(uint8_t), rom_a_size, fp);
	fclose(fp);

	if(bytes_written != rom_a_size)
	{
		printf("ERROR: Wrote %zu bytes when expected to write %zu bytes for merged ROM.  Aborting.\n", bytes_written, rom_a_size);
		return 1;
	}

	printf("Successfully wrote merged ROM.\n");

	return 0;
}

int swap_rom(const bool swap_state, const bool unconditional_swap, const bool encrypt_rom, const char* encryption_key_path, const bool correct_checksum, const char* rom_input_path, const char* rom_output_path)
{
	FILE *fp;
	size_t bytes_written = 0;
	uint8_t input_rom_buffer[MAX_AMIGA_ROM_SIZE];
	size_t input_rom_size = 0;
	size_t decrypted_input_rom_size = 0;
	size_t encrypted_input_rom_size = 0;
	const char* input_rom_version;

	input_rom_size = ReadAmigaROM(rom_input_path, input_rom_buffer);
	if(input_rom_size == 0)
	{
		printf("ERROR: Unable to open file: %s\n", rom_input_path);
		printf("Is it a valid ROM?\n");
		return 1;
	}

	if(DetectAmigaROMEncryption(input_rom_buffer))
	{
		if(encryption_key_path == NULL)
		{
			printf("ERROR: ROM is encrypted.  Specify a decryption key to decrypt.\n");
			return 1;
		}
		else
		{
			decrypted_input_rom_size = CryptAmigaROM(input_rom_buffer, input_rom_size, false, encryption_key_path);

			if(decrypted_input_rom_size == input_rom_size)
			{
				printf("ERROR: Unable to access ROM decryption key.\n");
				return 1;
			}
			else
			{
				input_rom_size = decrypted_input_rom_size;
			}
		}
	}

	input_rom_version = DetectAmigaROMVersion(input_rom_buffer, input_rom_size);
	if(input_rom_version == NULL)
	{
		printf("WARNING: Unknown ROM loaded.\n");
	}
	else
	{
		printf("Detected ROM: %s\n", input_rom_version);
	}

	if(SetAmigaROMByteSwap(input_rom_buffer, input_rom_size, swap_state, !swap_state, unconditional_swap) == 0)
	{
		printf("ERROR: Unable to perform conditional swap operation for ROM.  Aborting.\n");
		return 1;
	}

	if(encrypt_rom)
	{
		if(encryption_key_path == NULL)
		{
			printf("ERROR: Encrypting a ROM requires specifying an encryption key.\n");
			return 1;
		}
		encrypted_input_rom_size = CryptAmigaROM(input_rom_buffer, input_rom_size, true, encryption_key_path);

		if(encrypted_input_rom_size == input_rom_size)
		{
			printf("ERROR: Unable to access ROM encryption key.\n");
			return 1;
		}
		else
		{
			input_rom_size = encrypted_input_rom_size;
		}
	}

	fp = fopen(rom_output_path, "wb");
	if(!fp)
	{
		printf("ERROR: Unable to open file for writing swapped ROM: %s\n", rom_output_path);
		return 1;
	}

	bytes_written = fwrite(input_rom_buffer, sizeof(uint8_t), input_rom_size, fp);
	fclose(fp);

	if(bytes_written != input_rom_size)
	{
		printf("ERROR: Wrote %zu bytes when expected to write %zu bytes for swapped ROM.  Aborting.\n", bytes_written, input_rom_size);
		return 1;
	}

	printf("Successfully wrote swapped ROM.\n");

	return 0;
}

int crypt_rom(const bool encryption_state, const char* encryption_key_path, const char* rom_input_path, const char* rom_output_path)
{
	FILE *fp;
	size_t bytes_written = 0;
	uint8_t input_rom_buffer[MAX_AMIGA_ROM_SIZE];
	size_t input_rom_size = 0;
	bool current_encryption_state = false;
	size_t decrypted_input_rom_size = 0;
	size_t encrypted_input_rom_size = 0;
	const char* input_rom_version;

	input_rom_size = ReadAmigaROM(rom_input_path, input_rom_buffer);
	if(input_rom_size == 0)
	{
		printf("ERROR: Unable to open file: %s\n", rom_input_path);
		printf("Is it a valid ROM?\n");
		return 1;
	}

	current_encryption_state = DetectAmigaROMEncryption(input_rom_buffer);

	if(current_encryption_state && !encryption_state)
	{
		decrypted_input_rom_size = CryptAmigaROM(input_rom_buffer, input_rom_size, false, encryption_key_path);
		if(decrypted_input_rom_size == input_rom_size)
		{
			printf("ERROR: Unable to access ROM decryption key.\n");
			return 1;
		}
		else
		{
			printf("Decrypted ROM.\n");
			input_rom_size = decrypted_input_rom_size;
		}
	}

	input_rom_version = DetectAmigaROMVersion(input_rom_buffer, input_rom_size);
	if(input_rom_version == NULL)
	{
		printf("WARNING: Unknown ROM loaded.\n");
	}
	else
	{
		printf("Detected ROM: %s\n", input_rom_version);
	}

	if(!current_encryption_state && encryption_state)
	{
		encrypted_input_rom_size = CryptAmigaROM(input_rom_buffer, input_rom_size, true, encryption_key_path);
		if(encrypted_input_rom_size == input_rom_size)
		{
			printf("ERROR: Unable to access ROM encryption key.\n");
			return 1;
		}
		else
		{
			printf("Encrypted ROM.\n");
			input_rom_size = encrypted_input_rom_size;
		}
	}

	fp = fopen(rom_output_path, "wb");
	if(!fp)
	{
		printf("ERROR: Unable to open file for writing swapped ROM: %s\n", rom_output_path);
		return 1;
	}

	bytes_written = fwrite(input_rom_buffer, sizeof(uint8_t), input_rom_size, fp);
	fclose(fp);

	if(bytes_written != input_rom_size)
	{
		printf("ERROR: Wrote %zu bytes when expected to write %zu bytes for swapped ROM.  Aborting.\n", bytes_written, input_rom_size);
		return 1;
	}

	if(encryption_state)
	{
		printf("Successfully wrote encrypted ROM.\n");
	}
	else
	{
		printf("Successfully wrote decrypted ROM.\n");
	}

	return 0;
}

int checksum_rom(const bool correct_checksum, const char* encryption_key_path, const char* rom_input_path, const char* rom_output_path)
{
	FILE *fp;
	size_t bytes_written = 0;
	uint8_t input_rom_buffer[MAX_AMIGA_ROM_SIZE];
	size_t input_rom_size = 0;
	size_t decrypted_input_rom_size = 0;
	const char* input_rom_version;

	input_rom_size = ReadAmigaROM(rom_input_path, input_rom_buffer);
	if(input_rom_size == 0)
	{
		printf("ERROR: Unable to open file: %s\n", rom_input_path);
		printf("Is it a valid ROM?\n");
		return 1;
	}

	if(DetectAmigaROMEncryption(input_rom_buffer))
	{
		if(encryption_key_path == NULL)
		{
			printf("ERROR: ROM is encrypted.  Specify a decryption key to decrypt.\n");
			return 1;
		}
		else
		{
			decrypted_input_rom_size = CryptAmigaROM(input_rom_buffer, input_rom_size, false, encryption_key_path);

			if(decrypted_input_rom_size == input_rom_size)
			{
				printf("ERROR: Unable to access ROM decryption key.\n");
				return 1;
			}
			else
			{
				input_rom_size = decrypted_input_rom_size;
			}
		}
	}

	input_rom_version = DetectAmigaROMVersion(input_rom_buffer, input_rom_size);
	if(input_rom_version == NULL)
	{
		printf("WARNING: Unknown ROM loaded.\n");
	}
	else
	{
		printf("Detected ROM: %s\n", input_rom_version);
	}

	if(ValidateAmigaROMChecksum(input_rom_buffer, input_rom_size))
	{
		printf("ROM checksum is valid.\n");
	}
	else
	{
		if(correct_checksum)
		{
			if(CorrectAmigaROMChecksum(input_rom_buffer, input_rom_size))
			{
				printf("Corrected ROM checksum.\n");

				fp = fopen(rom_output_path, "wb");
				if(!fp)
				{
					printf("ERROR: Unable to open file for writing corrected ROM: %s\n", rom_output_path);
					return 1;
				}

				bytes_written = fwrite(input_rom_buffer, sizeof(uint8_t), input_rom_size, fp);
				fclose(fp);

				if(bytes_written != input_rom_size)
				{
					printf("ERROR: Wrote %zu bytes when expected to write %zu bytes for corrected ROM.  Aborting.\n", bytes_written, input_rom_size);
					return 1;
				}

				printf("Successfully wrote corrected ROM.\n");
			}
			else
			{
				printf("ERROR: Unable to correct ROM checksum.\n");
				return 1;
			}
		}
		else
		{
			printf("ERROR: ROM checksum is invalid.\n");
			return 1;
		}
	}

	return 0;
}
