#pragma once

#include <stdlib.h>
#include <stdint.h>

#if __cplusplus
extern "C" {
#endif

uint8_t * fun_CreateHuffmanCode( uint8_t * pDestDataP, uint32_t pDestDataLen, int * pRetLen );

#if __cplusplus
}
#endif
