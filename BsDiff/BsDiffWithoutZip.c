#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./BsDiffWithOutZip.h"

#define MIN(x,y) (((x)<(y)) ? (x) : (y))
typedef unsigned char   u_char;

void offtout(off_t x,u_char *buf)
{
    off_t y;

    if(x<0) y=-x; else y=x;

    buf[0]=y%256;y-=buf[0];
    y=y/256;buf[1]=y%256;y-=buf[1];
    y=y/256;buf[2]=y%256;y-=buf[2];
    y=y/256;buf[3]=y%256;y-=buf[3];
    y=y/256;buf[4]=y%256;y-=buf[4];
    y=y/256;buf[5]=y%256;y-=buf[5];
    y=y/256;buf[6]=y%256;y-=buf[6];
    y=y/256;buf[7]=y%256;

    if(x<0) buf[7]|=0x80;
}

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

off_t matchlen(u_char *old,off_t oldsize,u_char *new,off_t newsize)
{
    off_t i;

    for(i=0;(i<oldsize)&&(i<newsize);i++)
        if(old[i]!=new[i]) break;

    return i;
}

off_t search(off_t *I,u_char *old,off_t oldsize,
             u_char *new,off_t newsize,off_t st,off_t en,off_t *pos)
{
    off_t x,y;

    if(en-st<2) {
        x=matchlen(old+I[st],oldsize-I[st],new,newsize);
        y=matchlen(old+I[en],oldsize-I[en],new,newsize);

        if(x>y) {
            *pos=I[st];
            return x;
        } else {
            *pos=I[en];
            return y;
        }
    };

    x=st+(en-st)/2;
    if(memcmp(old+I[x],new,MIN(oldsize-I[x],newsize))<0) {
        return search(I,old,oldsize,new,newsize,x,en,pos);
    } else {
        return search(I,old,oldsize,new,newsize,st,x,pos);
    };
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
    int _index;
    u_char *old,*new;
    off_t oldsize,newsize;
    off_t *I,*V;
    u_char * _resultP;

    _ret = 0;
    _index = 0;
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
    free( V );

    _resultP = malloc( ( oldsize + newsize ) * 2 );

    off_t scan,pos,len;
    off_t lastscan,lastpos,lastoffset;
    off_t oldscore,scsc;

    off_t s,Sf,lenf,Sb,lenb;
    off_t overlap,Ss,lens;
    off_t i;

    off_t dblen,eblen;
    u_char *db,*eb;

    u_char buf[8];
    u_char header[32];

    if(((db=malloc(newsize+1))==NULL) ||
        ((eb=malloc(newsize+1))==NULL))
    {
        goto fun_StartBsDiff_exit;
    }
    dblen=0;
    eblen=0;

    memcpy(header,"BSDIFF40",8);
    memset(header+8,0,24);
    memcpy( _resultP, header, 32 );

    scan=0;len=0;
    lastscan=0;lastpos=0;lastoffset=0;
    while(scan<newsize) {
        oldscore=0;

        for(scsc=scan+=len;scan<newsize;scan++) {
            len=search(I,old,oldsize,new+scan,newsize-scan,
                         0,oldsize,&pos);

            for(;scsc<scan+len;scsc++)
                if((scsc+lastoffset<oldsize) &&
                    (old[scsc+lastoffset] == new[scsc]))
                    oldscore++;

            if(((len==oldscore) && (len!=0)) ||
                (len>oldscore+8)) break;

            if((scan+lastoffset<oldsize) &&
                (old[scan+lastoffset] == new[scan]))
                oldscore--;
        };

        if((len!=oldscore) || (scan==newsize)) {
            s=0;Sf=0;lenf=0;
            for(i=0;(lastscan+i<scan)&&(lastpos+i<oldsize);) {
                if(old[lastpos+i]==new[lastscan+i]) s++;
                i++;
                if(s*2-i>Sf*2-lenf) { Sf=s; lenf=i; };
            };

            lenb=0;
            if(scan<newsize) {
                s=0;Sb=0;
                for(i=1;(scan>=lastscan+i)&&(pos>=i);i++) {
                    if(old[pos-i]==new[scan-i]) s++;
                    if(s*2-i>Sb*2-lenb) { Sb=s; lenb=i; };
                };
            };

            if(lastscan+lenf>scan-lenb) {
                overlap=(lastscan+lenf)-(scan-lenb);
                s=0;Ss=0;lens=0;
                for(i=0;i<overlap;i++) {
                    if(new[lastscan+lenf-overlap+i]==
                        old[lastpos+lenf-overlap+i]) s++;
                    if(new[scan-lenb+i]==
                        old[pos-lenb+i]) s--;
                    if(s>Ss) { Ss=s; lens=i+1; };
                };

                lenf+=lens-overlap;
                lenb-=lens;
            };

            for(i=0;i<lenf;i++)
                db[dblen+i]=new[lastscan+i]-old[lastpos+i];
            for(i=0;i<(scan-lenb)-(lastscan+lenf);i++)
                eb[eblen+i]=new[lastscan+lenf+i];

            dblen+=lenf;
            eblen+=(scan-lenb)-(lastscan+lenf);

            offtout(lenf,buf);
            memcpy( &_resultP[ 32 + _index ], buf, 8 );
            _index += 8;

            offtout((scan-lenb)-(lastscan+lenf),buf);
            memcpy( &_resultP[ 32 + _index ], buf, 8 );
            _index += 8;

            offtout((pos-lenb)-(lastpos+lenf),buf);
            memcpy( &_resultP[ 32 + _index ], buf, 8 );
            _index += 8;

            lastscan=scan-lenb;
            lastpos=pos-lenb;
            lastoffset=pos-scan;
        };
    };

    off_t _tmpValue;

    _tmpValue = _index;

    offtout(_index,buf);
    memcpy( &_resultP[ 8 ], buf, 8 );

    offtout(newsize,buf);
    memcpy( &_resultP[ 24 ], buf, 8 );

    memcpy( &_resultP[ 32 + _index ], db, dblen );
    _index += dblen;

    offtout(_index + 32 - _tmpValue, buf);
    memcpy( &_resultP[ 16 ], buf, 8 );

    memcpy( &_resultP[ 32 + _index ], eb, eblen );
    _index += eblen;

    _ret = _index + 32;

    fun_StartBsDiff_exit:
    if( _ret == 0 )
    {
        free( _resultP );
    }
    else
    {
        *pBsDiffDataPP = _resultP;
    }
    return _ret;
}

/**
 * @brief sub_FreeMallocAddr
 * @param pDestP
 */
void sub_FreeMallocAddr( void * pDestP )
{
    free( pDestP );
}
