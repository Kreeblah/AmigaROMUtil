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

void print_help(void);
int print_rom_info(const char* encryption_key_path, const char* rom_input_path);
int split_rom(const bool swap, const bool unswap, const bool unconditional_swap, const char* encryption_key_path, const bool correct_checksum, const char* rom_high_path, const char* rom_low_path, const char* rom_input_path);
int merge_rom(const bool swap, const bool unswap, const bool unconditional_swap, const bool encrypt_rom, const char* encryption_key_path, const bool correct_checksum, const char* rom_high_path, const char* rom_low_path, const char* rom_output_path);
int swap_rom(const bool swap_state, const bool unconditional_swap, const bool encrypt_rom, const char* encryption_key_path, const bool correct_checksum, const char* rom_input_path, const char* rom_output_path);
int crypt_rom(const bool encryption_state, const char* encryption_key_path, const char* rom_input_path, const char* rom_output_path);
int checksum_rom(const bool correct_checksum, const char* encryption_key_path, const char* rom_input_path, const char* rom_output_path);

int main(int argc, char** argv)
{
	char* rom_input_path = NULL;
	char* rom_output_path = NULL;
	char* rom_high_path = NULL;
	char* rom_low_path = NULL;
	char* encryption_key_path = NULL;
	bool rom_info = false;
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

	while((c = getopt(argc, argv, "i:o:a:b:k:fsgpunvcedh")) != -1)
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
				rom_high_path = strdup(optarg);
				break;
			case 'b':
				rom_low_path = strdup(optarg);
				break;
			case 'k':
				encryption_key_path = strdup(optarg);
				break;
			case 'f':
				rom_info = true;
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

	if(!rom_info && !split && !merge && !swap && !unswap && !encrypt_rom && !decrypt_rom && !validate_checksum && !correct_checksum)
	{
		print_help();
		exit(1);
	}

	if(rom_info && !rom_input_path)
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

	if(split && (!rom_input_path || !rom_high_path || !rom_low_path))
	{
		print_help();
		exit(1);
	}

	if(merge && (!rom_output_path || !rom_high_path || !rom_low_path))
	{
		print_help();
		exit(1);
	}

	if(swap && (!rom_input_path || !rom_output_path))
	{
		print_help();
		exit(1);
	}

	if(unswap && (!rom_input_path || !rom_output_path))
	{
		print_help();
		exit(1);
	}

	if((encrypt_rom || decrypt_rom || correct_checksum) && (!rom_input_path || !rom_output_path))
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

	if(rom_info)
	{
		operation_result = print_rom_info(encryption_key_path, rom_input_path);
	}
	else if(split)
	{
		operation_result = split_rom(swap, unswap, unconditional_swap, encryption_key_path, correct_checksum, rom_high_path, rom_low_path, rom_input_path);
	}
	else if(merge)
	{
		operation_result = merge_rom(swap, unswap, unconditional_swap, encrypt_rom, encryption_key_path, correct_checksum, rom_high_path, rom_low_path, rom_output_path);
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

	free(rom_high_path);
	free(rom_low_path);
	free(rom_input_path);
	free(rom_output_path);
	free(encryption_key_path);

	exit(operation_result);
}

void print_help(void)
{
    printf("Usage: AmigaROMUtil [options]\n");
    printf("Options:\n");
    printf("  -i FILE  Path to input ROM (except for merging)\n");
    printf("  -o FILE  Path to output ROM (except for splitting)\n");
    printf("  -a FILE  Path to High ROM for merging or splitting\n");
    printf("  -b FILE  Path to Low ROM for merging or splitting\n");
    printf("  -k FILE  Path to ROM encryption/decryption key\n");
    printf("  -f       Print ROM info and quit (requires -i)\n");
    printf("  -s       Split ROM (requires -i, -a, -b)\n");
    printf("  -g       Merge ROM (requires -a, -b, -o)\n");
    printf("  -p       Byte swap ROM for burning to an IC (requires -i, -o)\n");
    printf("  -u       Unbyte swap ROM for use with emulators (requires -i, -o)\n");
    printf("  -n       Unconditional byte swap for ROMs which are unknown to this program\n");
    printf("  -v       Validate checksum (requires -i)\n");
    printf("  -c       Correct checksum (requires -i, -o)\n");
    printf("  -e       Encrypt ROM (requires -i, -o, -k)\n");
    printf("  -d       Decrypt ROM (requires -i, -o, -k)\n");
    printf("  -h       Display this information\n");
    printf("\n");
    printf("Notes:\n");
    printf("-n implies -u and not -n, and will override those if set\n");
    printf("-s and -m, -p and -u, -e and -d are each mutually exclusive\n");
	return;
}

int print_rom_info(const char* encryption_key_path, const char* rom_input_path)
{
	char *info_string = NULL;
	ParsedAmigaROMData input_rom = GetInitializedAmigaROM();

	info_string = (char *)malloc(4096);
	if(!info_string)
	{
		return 1;
	}

	input_rom = ReadAmigaROM(rom_input_path, encryption_key_path);
	PrintAmigaROMInfo(&input_rom, info_string, 4096);

	printf("%s\n", info_string);

	free(info_string);

	return 0;
}

int split_rom(const bool swap, const bool unswap, const bool unconditional_swap, const char* encryption_key_path, const bool correct_checksum, const char* rom_high_path, const char* rom_low_path, const char* rom_input_path)
{
	ParsedAmigaROMData high_rom = GetInitializedAmigaROM();
	ParsedAmigaROMData input_rom = GetInitializedAmigaROM();
	ParsedAmigaROMData low_rom = GetInitializedAmigaROM();

	input_rom = ReadAmigaROM(rom_input_path, encryption_key_path);
	if(!input_rom.parsed_rom)
	{
		if(input_rom.is_encrypted && !input_rom.can_decrypt)
		{
			printf("ERROR: Source ROM is encrypted.  Please provide a valid key to decrypt.");
		}
		else
		{
			printf("ERROR: Unable to load source ROM at: %s\n", rom_high_path);
			printf("Is it a valid ROM?\n");
		}

		if(input_rom.rom_data)
		{
			DestroyInitializedAmigaROM(&input_rom);
		}

		return 1;
	}

	if(input_rom.version == NULL)
	{
		printf("WARNING: Unknown source ROM loaded.\n");
	}
	else
	{
		printf("Detected source ROM: %s\n", input_rom.version);
	}

	if(input_rom.type != 'M')
	{
		printf("ROM type: %c\n", input_rom.type);
		printf("WARNING: ROM is not detected as a known merged ROM.\n");
	}

	if(input_rom.has_valid_checksum)
	{
		printf("Source ROM checksum is valid.\n");
	}
	else
	{
		if(correct_checksum)
		{
			if(CorrectAmigaROMChecksum(&input_rom))
			{
				printf("Corrected source ROM checksum.\n");
			}
			else
			{
				DestroyInitializedAmigaROM(&input_rom);
				printf("ERROR: Unable to correct source ROM checksum.\n");
				return 1;
			}
		}
		else
		{
			printf("WARNING: Source ROM checksum is invalid.\n");
		}
	}

	if((swap || unswap) && ((input_rom.type != 'U' || unconditional_swap) || SetAmigaROMByteSwap(&input_rom, swap, unswap, unconditional_swap) == 0))
	{
		DestroyInitializedAmigaROM(&input_rom);
		printf("ERROR: Unable to perform conditional swap operation.  Aborting.\n");
		return 1;
	}

	if(!SplitAmigaROM(&input_rom, &high_rom, &low_rom))
	{
		DestroyInitializedAmigaROM(&input_rom);

		if(high_rom.rom_data)
		{
			DestroyInitializedAmigaROM(&high_rom);
		}

		if(low_rom.rom_data)
		{
			DestroyInitializedAmigaROM(&low_rom);
		}

		printf("ERROR: ROM split operation failed.  Aborting.\n");
		return 1;
	}

	if(!WriteAmigaROM(&high_rom, rom_high_path))
	{
		DestroyInitializedAmigaROM(&input_rom);
		DestroyInitializedAmigaROM(&high_rom);
		DestroyInitializedAmigaROM(&low_rom);
		printf("ERROR: Unable to write High ROM to disk.  Aborting.\n");
		return 1;
	}

	if(!WriteAmigaROM(&low_rom, rom_low_path))
	{
		DestroyInitializedAmigaROM(&input_rom);
		DestroyInitializedAmigaROM(&high_rom);
		DestroyInitializedAmigaROM(&low_rom);
		printf("ERROR: Unable to write Low ROM to disk.  Aborting.\n");
		return 1;
	}

	DestroyInitializedAmigaROM(&input_rom);
	DestroyInitializedAmigaROM(&high_rom);
	DestroyInitializedAmigaROM(&low_rom);
	printf("Successfully wrote High and Low ROMs.\n");

	return 0;
}

int merge_rom(const bool swap, const bool unswap, const bool unconditional_swap, const bool encrypt_rom, const char* encryption_key_path, const bool correct_checksum, const char* rom_high_path, const char* rom_low_path, const char* rom_output_path)
{
	ParsedAmigaROMData high_rom = GetInitializedAmigaROM();
	ParsedAmigaROMData low_rom = GetInitializedAmigaROM();
	ParsedAmigaROMData output_rom = GetInitializedAmigaROM();

	high_rom = ReadAmigaROM(rom_high_path, encryption_key_path);
	if(!high_rom.parsed_rom)
	{
		if(high_rom.is_encrypted && !high_rom.can_decrypt)
		{
			printf("ERROR: High ROM is encrypted.  Please provide a valid key to decrypt.");
		}
		else
		{
			printf("ERROR: Unable to load High ROM at: %s\n", rom_high_path);
			printf("Is it a valid ROM?\n");
		}

		if(high_rom.rom_data)
		{
			DestroyInitializedAmigaROM(&high_rom);
		}

		return 1;
	}

	low_rom = ReadAmigaROM(rom_low_path, encryption_key_path);
	if(!low_rom.parsed_rom)
	{
		if(low_rom.is_encrypted && !low_rom.can_decrypt)
		{
			printf("ERROR: Low ROM is encrypted.  Please provide a valid key to decrypt.");
		}
		else
		{
			printf("ERROR: Unable to load Low ROM at: %s\n", rom_low_path);
			printf("Is it a valid ROM?\n");
		}

		if(low_rom.rom_data)
		{
			DestroyInitializedAmigaROM(&low_rom);
		}

		DestroyInitializedAmigaROM(&high_rom);
		return 1;
	}

	if(high_rom.rom_size != low_rom.rom_size)
	{
		printf("ERROR: High and Low ROMs must be the same size to merge.\n");
		return 1;
	}

	if(high_rom.version == NULL)
	{
		printf("WARNING: Unknown High ROM loaded.\n");
	}
	else
	{
		printf("Detected High ROM: %s\n", high_rom.version);
	}

	if(low_rom.version == NULL)
	{
		printf("WARNING: Unknown Low ROM loaded.\n");
	}
	else
	{
		printf("Detected Low ROM: %s\n", low_rom.version);
	}

	if(high_rom.version != NULL && low_rom.version == NULL)
	{
		printf("WARNING: Known High ROM and unknown Low ROM detected.\n");
	}
	else if(low_rom.version != NULL && high_rom.version == NULL)
	{
		printf("WARNING: Known Low ROM and unknown High ROM detected.\n");
	}
	else if((high_rom.version != NULL && low_rom.version != NULL) && (strlen(high_rom.version) != strlen(low_rom.version) || strncmp(high_rom.version, low_rom.version, strlen(high_rom.version)) != 0))
	{
		printf("WARNING: High and Low ROMs are not from the same set.\n");
	}

	if(high_rom.type != 'A')
	{
		printf("WARNING: High ROM is not detected as a known High ROM.\n");
	}

	if(low_rom.type != 'B')
	{
		printf("WARNING: Low ROM is not detected as a known Low ROM.\n");
	}

	if((swap || unswap) && ((high_rom.type != 'U' || unconditional_swap) && SetAmigaROMByteSwap(&high_rom, swap, unswap, unconditional_swap) == 0))
	{
		DestroyInitializedAmigaROM(&high_rom);
		DestroyInitializedAmigaROM(&low_rom);
		printf("ERROR: Unable to perform conditional swap operation for High ROM.  Aborting.\n");
		return 1;
	}

	if((swap || unswap) && ((low_rom.type != 'U' || unconditional_swap) && SetAmigaROMByteSwap(&low_rom, swap, unswap, unconditional_swap) == 0))
	{
		DestroyInitializedAmigaROM(&high_rom);
		DestroyInitializedAmigaROM(&low_rom);
		printf("ERROR: Unable to perform conditional swap operation for Low ROM.  Aborting.\n");
		return 1;
	}

	if(!MergeAmigaROM(&high_rom, &low_rom, &output_rom))
	{
		if(output_rom.rom_data)
		{
			DestroyInitializedAmigaROM(&output_rom);
		}

		DestroyInitializedAmigaROM(&high_rom);
		DestroyInitializedAmigaROM(&low_rom);
		printf("ERROR: ROM merge operation failed.  Aborting.\n");
		return 1;
	}

	ParseAmigaROMData(&output_rom, encryption_key_path);

	if(output_rom.has_valid_checksum)
	{
		printf("Merged ROM checksum is valid.\n");
	}
	else
	{
		if(correct_checksum)
		{
			if(CorrectAmigaROMChecksum(&output_rom))
			{
				printf("Corrected merged ROM checksum.\n");
			}
			else
			{
				DestroyInitializedAmigaROM(&output_rom);
				DestroyInitializedAmigaROM(&high_rom);
				DestroyInitializedAmigaROM(&low_rom);
				printf("ERROR: Unable to correct merged ROM checksum.\n");
				return 1;
			}
		}
		else
		{
			printf("WARNING: Merged ROM checksum is invalid.\n");
		}
	}

	if(encrypt_rom)
	{
		if(encryption_key_path == NULL)
		{
			DestroyInitializedAmigaROM(&output_rom);
			DestroyInitializedAmigaROM(&high_rom);
			DestroyInitializedAmigaROM(&low_rom);
			printf("ERROR: Encrypting a ROM requires specifying an encryption key.\n");
			return 1;
		}

		if(high_rom.is_encrypted)
		{
			printf("INFO: Encrypting merged ROM with the same key as the High ROM.\n");
		}
		if(low_rom.is_encrypted)
		{
			printf("INFO: Encrypting merged ROM with the same key as the Low ROM.\n");
		}

		if(!CryptAmigaROM(&output_rom, true, encryption_key_path))
		{
			DestroyInitializedAmigaROM(&output_rom);
			DestroyInitializedAmigaROM(&high_rom);
			DestroyInitializedAmigaROM(&low_rom);
			printf("ERROR: Unable to access ROM encryption key.\n");
			return 1;
		}
		else
		{
			printf("Encrypted ROM.\n");
		}
	}

	if(!WriteAmigaROM(&output_rom, rom_output_path))
	{
		DestroyInitializedAmigaROM(&output_rom);
		DestroyInitializedAmigaROM(&high_rom);
		DestroyInitializedAmigaROM(&low_rom);
		printf("ERROR: Unable to write merged ROM to disk.  Aborting.");
		return 1;
	}

	DestroyInitializedAmigaROM(&output_rom);
	DestroyInitializedAmigaROM(&high_rom);
	DestroyInitializedAmigaROM(&low_rom);
	printf("Successfully wrote merged ROM.\n");

	return 0;
}

int swap_rom(const bool swap_state, const bool unconditional_swap, const bool encrypt_rom, const char* encryption_key_path, const bool correct_checksum, const char* rom_input_path, const char* rom_output_path)
{
	size_t encrypted_input_rom_size = 0;
	ParsedAmigaROMData input_rom = GetInitializedAmigaROM();

	input_rom = ReadAmigaROM(rom_input_path, encryption_key_path);
	if(!input_rom.parsed_rom)
	{
		if(input_rom.is_encrypted && !input_rom.can_decrypt)
		{
			printf("ERROR: Source ROM is encrypted.  Please provide a valid key to decrypt.");
		}
		else
		{
			printf("ERROR: Unable to load source ROM at: %s\n", rom_input_path);
			printf("Is it a valid ROM?\n");
		}

		if(input_rom.rom_data != NULL)
		{
			DestroyInitializedAmigaROM(&input_rom);
		}

		return 1;
	}

	if(input_rom.version == NULL)
	{
		printf("WARNING: Unknown source ROM loaded.\n");
	}
	else
	{
		printf("Detected source ROM: %s\n", input_rom.version);
	}

	if(SetAmigaROMByteSwap(&input_rom, swap_state, !swap_state, unconditional_swap) == 0)
	{
		DestroyInitializedAmigaROM(&input_rom);
		printf("ERROR: Unable to perform conditional swap operation for ROM.  Aborting.\n");
		return 1;
	}

	if(input_rom.has_valid_checksum)
	{
		printf("ROM checksum is valid.\n");
	}
	else
	{
		if(correct_checksum)
		{
			if(CorrectAmigaROMChecksum(&input_rom))
			{
				printf("Corrected ROM checksum.\n");
			}
			else
			{
				DestroyInitializedAmigaROM(&input_rom);
				printf("ERROR: Unable to correct checksum for ROM.  Aborting.\n");
				return 1;
			}
		}
		else
		{
			printf("WARNING: ROM checksum is invalid.\n");
		}
	}

	if(encrypt_rom)
	{
		if(encryption_key_path == NULL)
		{
			DestroyInitializedAmigaROM(&input_rom);
			printf("ERROR: Encrypting a ROM requires specifying an encryption key.\n");
			return 1;
		}

		if(input_rom.is_encrypted)
		{
			printf("INFO: Encrypting swapped ROM with the same key as the source ROM.\n");
		}

		if(!CryptAmigaROM(&input_rom, true, encryption_key_path))
		{
			DestroyInitializedAmigaROM(&input_rom);
			printf("ERROR: Unable to access ROM encryption key.\n");
			return 1;
		}
		else
		{
			printf("Encrypted ROM.\n");
			input_rom.rom_size = encrypted_input_rom_size;
		}
	}

	if(!WriteAmigaROM(&input_rom, rom_output_path))
	{
		DestroyInitializedAmigaROM(&input_rom);
		printf("ERROR: Unable to write swapped ROM to disk.\n");
		return 1;
	}

	DestroyInitializedAmigaROM(&input_rom);

	if(swap_state)
	{
		printf("Successfully wrote swapped ROM.\n");
	}
	else
	{
		printf("Successfully wrote unswapped ROM.\n");
	}

	return 0;
}

int crypt_rom(const bool encryption_state, const char* encryption_key_path, const char* rom_input_path, const char* rom_output_path)
{
	size_t encrypted_input_rom_size = 0;

	ParsedAmigaROMData input_rom = GetInitializedAmigaROM();

	input_rom = ReadAmigaROM(rom_input_path, encryption_key_path);
	if(!input_rom.parsed_rom)
	{
		if(input_rom.is_encrypted && !input_rom.can_decrypt)
		{
			printf("ERROR: Source ROM is encrypted.  Please provide a valid key to decrypt.");
		}
		else
		{
			printf("ERROR: Unable to load source ROM at: %s\n", rom_input_path);
			printf("Is it a valid ROM?\n");
		}

		if(input_rom.rom_data != NULL)
		{
			DestroyInitializedAmigaROM(&input_rom);
		}

		return 1;
	}

	if(input_rom.version == NULL)
	{
		printf("WARNING: Unknown source ROM loaded.\n");
	}
	else
	{
		printf("Detected source ROM: %s\n", input_rom.version);
	}

	if(input_rom.is_encrypted && input_rom.successfully_decrypted && !encryption_state)
	{
		{
			printf("Decrypted ROM.\n");
		}
	}

	if(encryption_state)
	{
		if(input_rom.is_encrypted)
		{
			DestroyInitializedAmigaROM(&input_rom);
			printf("ERROR: Encrypting with the same key used to decrypt the ROM.\n");
			return 1;
		}

		if(!CryptAmigaROM(&input_rom, true, encryption_key_path))
		{
			DestroyInitializedAmigaROM(&input_rom);
			printf("ERROR: Unable to access ROM encryption key.\n");
			return 1;
		}
		else
		{
			printf("Encrypted ROM.\n");
			input_rom.rom_size = encrypted_input_rom_size;
		}
	}

	if(!WriteAmigaROM(&input_rom, rom_output_path))
	{
		DestroyInitializedAmigaROM(&input_rom);

		if(encryption_state)
		{
			printf("ERROR: Unable to write encrypted ROM to disk.\n");
		}
		else
		{
			printf("ERROR: Unable to write decrypted ROM to disk.\n");
		}

		return 1;
	}

	DestroyInitializedAmigaROM(&input_rom);
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
	ParsedAmigaROMData input_rom;

	input_rom = ReadAmigaROM(rom_input_path, encryption_key_path);
	if(!input_rom.parsed_rom)
	{
		if(input_rom.is_encrypted && !input_rom.can_decrypt)
		{
			printf("ERROR: Source ROM is encrypted.  Please provide a valid key to decrypt.");
		}
		else
		{
			printf("ERROR: Unable to load source ROM at: %s\n", rom_input_path);
			printf("Is it a valid ROM?\n");
		}

		if(input_rom.rom_data != NULL)
		{
			DestroyInitializedAmigaROM(&input_rom);
		}

		return 1;
	}

	if(input_rom.version == NULL)
	{
		printf("WARNING: Unknown source ROM loaded.\n");
	}
	else
	{
		printf("Detected source ROM: %s\n", input_rom.version);
	}

	if(input_rom.has_valid_checksum)
	{
		printf("ROM checksum is valid.\n");
	}
	else
	{
		if(correct_checksum)
		{
			if(CorrectAmigaROMChecksum(&input_rom))
			{
				printf("Corrected ROM checksum.\n");

				if(!WriteAmigaROM(&input_rom, rom_output_path))
				{
					DestroyInitializedAmigaROM(&input_rom);
					printf("ERROR: Unable to write corrected ROM to disk.\n");
					return 1;
				}

				printf("Successfully wrote corrected ROM.\n");
			}
			else
			{
				DestroyInitializedAmigaROM(&input_rom);
				printf("ERROR: Unable to correct ROM checksum.\n");
				return 1;
			}
		}
		else
		{
			DestroyInitializedAmigaROM(&input_rom);
			printf("ERROR: ROM checksum is invalid.\n");
			return 1;
		}
	}

	DestroyInitializedAmigaROM(&input_rom);

	return 0;
}
