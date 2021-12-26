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

#ifndef AMIGAROMMAJORMINORVERSIONS_H
#define AMIGAROMMAJORMINORVERSIONS_H

#include <stdint.h>

typedef struct {
	const uint16_t major_version;
	const uint16_t minor_version; // 0xffff is a "don't care", as the major_version has a 1:1 match with a version
	const char version[128];
} AmigaROMMajorMinorVersions;

static const AmigaROMMajorMinorVersions AMIGA_ROM_MAJOR_MINOR_VERSIONS[] = {
	{23, 0xffff, "AmigaOS \"Velvet\" prerelease"},
	{24, 0xffff, "AmigaOS 0.4 (A1000)"},
	{26, 0xffff, "AmigaOS 0.6 (A1000)"},
	{27, 0xffff, "AmigaOS 0.7 (A1000)"},
	{29, 0xffff, "AmigaOS 0.9 (A1000)"},
	{30, 0xffff, "AmigaOS 1.0 (A1000)"},
	{31, 0xffff, "AmigaOS 1.1 (A1000)"},
	{32, 0xffff, "AmigaOS 1.1 (A1000)"},
	{33, 0xffff, "AmigaOS 1.2 (A500/A1000/A2000)"},
	{34, 0xffff, "AmigaOS 1.3 (A500/A1000/A2000/A3000/CDTV)"},
	{35, 0xffff, "AmigaOS 1.3 (A2024)"},
	{36, 0xffff, "AmigaOS 2.0 (A500+ ECS/A3000)"},
	{37,    175, "AmigaOS 2.04 (A500+/A2000/A3000)"},
	{37,    210, "AmigaOS 2.05 (A600)"},
	{37,    299, "AmigaOS 2.05 (A600 ECS)"},
	{37,    300, "AmigaOS 2.05 (A600HD)"},
	{37,    350, "AmigaOS 2.05 (A600HD)"},
	{38, 0xffff, "AmigaOS 2.1"},
	{39, 0xffff, "AmigaOS 3.0 (A1200/A4000)"},
	{40, 0xffff, "AmigaOS 3.1 (A500/A600/A1200/A2000/A3000/A4000/A4000T/CD32)"},
	{41, 0xffff, "AmigaOS 3.1 (Japan localization)"},
	{43, 0xffff, "AmigaOS 3.1 (patched) / AmigaOS 3.2 (Walker prototype)"},
	{44, 0xffff, "Haage & Partner AmigaOS 3.5"},
	{45, 0xffff, "Cloanto AmigaOS 3.x / Haage & Partner AmigaOS 3.9"},
	{46, 0xffff, "Hyperion Entertainment AmigaOS 3.1.4"},
	{47, 0xffff, "Hyperion Entertainment AmigaOS 3.2"},
	{50, 0xffff, "Hyperion Entertainment AmigaOS 4 Beta / MorphOS 1"},
	{51, 0xffff, "Hyperion Entertainment AmigaOS 4 Beta / MorphOS 2+"},
	{52, 0xffff, "Hyperion Entertainment AmigaOS 4.0"},
	{53, 0xffff, "Hyperion Entertainment AmigaOS 4.1"}
};

#endif
