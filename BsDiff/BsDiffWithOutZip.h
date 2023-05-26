#ifndef BSDIFFWITHOUTZIP_H
#define BSDIFFWITHOUTZIP_H

#include <stdint.h>

#if __cplusplus
extern "C" {
#endif

int fun_StartBsDiff( uint8_t * pOldFileDataP, int pOldFileLen, uint8_t * pNewFileDataP, int pNewFileLen, uint8_t ** pBsDiffDataPP );
void sub_FreeMallocAddr( void * pDestP );

#if __cplusplus
}
#endif

#endif // BSDIFFWITHOUTZIP_H
