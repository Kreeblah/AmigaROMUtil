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

#ifndef _AMIGAROMHASHES_H
#define _AMIGAROMHASHES_H

#include <stddef.h>

static const size_t AMIGA_ROM_QUANTITY = 38;

struct AmigaROMInfo {
	char sha1hash[41];
	size_t file_size;
	char version[128];
	char type;
	int byte_swap;
};

static const struct AmigaROMInfo AMIGA_ROM_INFO[AMIGA_ROM_QUANTITY] = {
	{"c87f9fada4ee4e69f3cca0c36193be822b9f5fe6", 8192, "Amiga Boot (A1000)", 'M', 0},
	{"e50f01baf28998928554786372a82c333c91276e", 16384, "Amiga Boot (A590)", 'M', 0},
	{"3ce66919f6fd67974923a12d91b730f1ffb4a7ba", 32768, "Amiga Boot (A4091)", 'M', 0},
	{"fa685ebb666c3d8b09afe1329b7e41b443312f69", 262144, "AmigaOS 0.7", 'M', 0},
	{"00c15406beb4b8ab1a16aa66c05860e1a7c1ad79", 262144, "AmigaOS 1.0", 'M', 0},
	{"4192c505d130f446b2ada6bdc91dae730acafb4c", 262144, "AmigaOS 1.1 (NTSC)", 'M', 0},
	{"16df8b5fd524c5a1c7584b2457ac15aff9e3ad6d", 262144, "AmigaOS 1.1 (PAL)", 'M', 0},
	{"11f9e62cf299f72184835b7b2a70a16333fc0d88", 262144, "AmigaOS 1.2", 'M', 0},
	{"891e9a547772fe0c6c19b610baf8bc4ea7fcb785", 262144, "AmigaOS 1.3", 'M', 0},
	{"ed7e461d1fff3cda321631ae42b80e3cd4fa5ebb", 262144, "AmigaOS 1.3 (A570 Ext.)", 'M', 0},
	{"c39bd9094d4e5f4e28c1411f3086950406062e87", 262144, "AmigaOS 1.3 (A3000)", 'M', 0},
	{"7ba40ffa17e500ed9fed041f3424bd81d9c907be", 262144, "AmigaOS 1.3 (CDTV Ext.)", 'M', 0},
	{"f76316bf36dff14b20fa349ed02e4b11dd932b07", 524288, "AmigaOS 1.4 (A3000)", 'M', 0},
	{"c5839f5cb98a7a8947065c3ed2f14f5f42e334a1", 524288, "AmigaOS 2.04", 'M', 0},
	{"d82ebb59afc53540ddf2d7187ecf239b7ea91590", 524288, "AmigaOS 2.04 (A3000)", 'M', 0},
	{"02843c4253bbd29aba535b0aa3bd9a85034ecde4", 524288, "AmigaOS 2.05 (A600)", 'M', 0},
	{"14271106fe97e655345ba8cccac52be999c37990", 262144, "AmigaOS 2.3 (CDTV Ext.)", 'M', 0},
	{"70033828182fffc7ed106e5373a8b89dda76faa5", 524288, "AmigaOS 3.0 (A1200)", 'M', 0},
	{"f0b4e9e29e12218c2d5bd7020e4e785297d91fd7", 524288, "AmigaOS 3.0 (A4000)", 'M', 0},
	{"3b7f1493b27e212830f989f26ca76c02049f09ca", 524288, "AmigaOS 3.1 (A600)", 'M', 0},
	{"e21545723fe8374e91342617604f1b3d703094f1", 524288, "AmigaOS 3.1 (A1200)", 'M', 0},
	{"f8e210d72b4c4853e0c9b85d223ba20e3d1b36ee", 524288, "AmigaOS 3.1 (A3000)", 'M', 0},
	{"5fe04842d04a489720f0f4bb0e46948199406f49", 524288, "AmigaOS 3.1 (A4000 Desktop)", 'M', 0},
	{"b0ec8b84d6768321e01209f11e6248f2f5281a21", 524288, "AmigaOS 3.1 (A4000 Tower)", 'M', 0},
	{"3525be8887f79b5929e017b42380a79edfee542d", 524288, "AmigaOS 3.1 (CD32)", 'M', 0},
	{"5bef3d628ce59cc02a66e6e4ae0da48f60e78f7f", 524288, "AmigaOS 3.1 (CD32 Ext.)", 'M', 0},
	{"03ca81c7a7b259cf64bc9582863eca0f6529f435", 262144, "Amiga CRT 3.1 CD32 FMV", 'O', 0},
	{"7a9095f1107966f90267dc4cb3c1972efb4b78a8", 524288, "AmigaOS 3.2 (Walker)", 'M', 0},
	{"46f15c7a50b45eed7cc1a1670b164aff71b01907", 524288, "AmigaOS 3.x (A600)", 'M', 0},
	{"279c6a27f8cb81c74e10b263d20f03433f98dc07", 524288, "AmigaOS 3.x (A1200)", 'M', 0},
	{"10390d4169b1009e6c81f8a572cddd83127f583d", 524288, "AmigaOS 3.x (A3000)", 'M', 0},
	{"0e1a4b5062826d8e3520cce752f47409f4adef96", 524288, "AmigaOS 3.x (A4000 Desktop)", 'M', 0},
	{"d5f1be640c38e7a8585567e4ed71b7d5e1ff772c", 524288, "AmigaOS 3.x (A4000 Tower)", 'M', 0},
	{"0a2cd16c696f62c02a146a95223450f37624f0ff", 524288, "AROS 20210207 (Ext.)", 'M', 0},
	{"fcf96487612170c58b4e0527db80ecd24572fbe5", 524288, "AROS 20210207", 'M', 0},
	{"ba10d16166b2e2d6177c979c99edf8462b21651e", 524288, "Logica Dialoga 2.0", 'O', 0},
	{"9e292178d20a6773f90664b00b137d8f4e52233d", 131072, "Ralph Schmidt Cyberstorm PPC 44.71", 'O', 0},
	{"cafafb916f16b9f3ec9b49aa4b40eb4eeceb5b5b", 131072, "Village Tronic Picasso IV 7.4", 'O', 0}
};

#endif