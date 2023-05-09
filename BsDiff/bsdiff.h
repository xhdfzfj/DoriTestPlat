#ifndef __BSDIFF_H__
#define __BSDIFF_H__

#include <stdint.h>

#if __cplusplus
extern "C" {
#endif

int fun_StartBsDiff( uint8_t * pOldFileDataP, int pOldFileLen, uint8_t * pNewFileDataP, int pNewFileLen, uint8_t ** pBsDiffDataPP );

#if __cplusplus
}
#endif

typedef uint8_t u_char;

#endif
