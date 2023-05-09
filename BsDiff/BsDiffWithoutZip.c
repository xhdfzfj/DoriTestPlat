#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./bsdiff.h"

#define MIN(x,y) (((x)<(y)) ? (x) : (y))

void split(off_t *I,off_t *V,off_t start,off_t len,off_t h)
{
    off_t i,j,k,x,tmp,jj,kk;

    if(len<16) {
        for(k=start;k<start+len;k+=j) {
            j=1;x=V[I[k]+h];
            for(i=1;k+i<start+len;i++) {
                if(V[I[k+i]+h]<x) {
                    x=V[I[k+i]+h];
                    j=0;
                };
                if(V[I[k+i]+h]==x) {
                    tmp=I[k+j];I[k+j]=I[k+i];I[k+i]=tmp;
                    j++;
                };
            };
            for(i=0;i<j;i++) V[I[k+i]]=k+j-1;
            if(j==1) I[k]=-1;
        };
        return;
    };

    x=V[I[start+len/2]+h];
    jj=0;kk=0;
    for(i=start;i<start+len;i++) {
        if(V[I[i]+h]<x) jj++;
        if(V[I[i]+h]==x) kk++;
    };
    jj+=start;kk+=jj;

    i=start;j=0;k=0;
    while(i<jj) {
        if(V[I[i]+h]<x) {
            i++;
        } else if(V[I[i]+h]==x) {
            tmp=I[i];I[i]=I[jj+j];I[jj+j]=tmp;
            j++;
        } else {
            tmp=I[i];I[i]=I[kk+k];I[kk+k]=tmp;
            k++;
        };
    };

    while(jj+j<kk) {
        if(V[I[jj+j]+h]==x) {
            j++;
        } else {
            tmp=I[jj+j];I[jj+j]=I[kk+k];I[kk+k]=tmp;
            k++;
        };
    };

    if(jj>start) split(I,V,start,jj-start,h);

    for(i=0;i<kk-jj;i++) V[I[jj+i]]=kk-1;
    if(jj==kk-1) I[jj]=-1;

    if(start+len>kk) split(I,V,kk,start+len-kk,h);
}


void qsufsort(off_t *I,off_t *V,u_char *old,off_t oldsize)
{
    off_t buckets[256];
    off_t i,h,len;

    for(i=0;i<256;i++) buckets[i]=0;
    for(i=0;i<oldsize;i++) buckets[old[i]]++;
    for(i=1;i<256;i++) buckets[i]+=buckets[i-1];
    for(i=255;i>0;i--) buckets[i]=buckets[i-1];
    buckets[0]=0;

    for(i=0;i<oldsize;i++) I[++buckets[old[i]]]=i;
    I[0]=oldsize;
    for(i=0;i<oldsize;i++) V[i]=buckets[old[i]];
    V[oldsize]=0;
    for(i=1;i<256;i++) if(buckets[i]==buckets[i-1]+1) I[buckets[i]]=-1;
    I[0]=-1;

    for(h=1;I[0]!=-(oldsize+1);h+=h) {
        len=0;
        for(i=0;i<oldsize+1;) {
            if(I[i]<0) {
                len-=I[i];
                i-=I[i];
            } else {
                if(len) I[i-len]=-len;
                len=V[I[i]]+1-i;
                split(I,V,i,len,h);
                i+=len;
                len=0;
            };
        };
        if(len) I[i-len]=-len;
    };

    for(i=0;i<oldsize+1;i++) I[V[i]]=i;
}


/**
 * @brief fun_StartBsDiff
 * @param pOldFileDataP
 * @param pOldFileLen
 * @param pNewFileDataP
 * @param pNewFileLen
 * @return
 */
int fun_StartBsDiff( uint8_t * pOldFileDataP, int pOldFileLen, uint8_t * pNewFileDataP, int pNewFileLen, uint8_t ** pBsDiffDataPP )
{
    int _ret;
    u_char *old,*new;
    off_t oldsize,newsize;
    off_t *I,*V;

    _ret = 0;
    oldsize = pOldFileLen;
    newsize = pNewFileLen;
    old = pOldFileDataP;
    new = pNewFileDataP;

    if(((I=malloc((oldsize+1)*sizeof(off_t)))==NULL) ||
        ((V=malloc((oldsize+1)*sizeof(off_t)))==NULL))
    {
        goto fun_StartBsDiff_exit;
    }

    qsufsort( I, V, old, oldsize );

    fun_StartBsDiff_exit:
    return _ret;
}
