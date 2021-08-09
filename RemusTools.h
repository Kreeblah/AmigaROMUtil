#ifndef REMUSTOOLS_H
#define REMUSTOOLS_H

#include <stdint.h>

#define REMUS_SPLIT_FILE_HEADER              0x524D5346
#define REMUS_SPLIT_FILE_VERSION_5           0x00020005
#define REMUS_SPLIT_FILE_VERSION_7           0x00020007

typedef struct RemusFile {
	uint32_t header;
	uint32_t version;
	uint8_t  *file_data;
}

#endif
