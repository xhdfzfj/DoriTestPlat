#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "Rtu.bin"

#define BUFSIZE 16

typedef struct _HNode
{
	char val;
	int left;
	int right;
	
}HNode;

typedef struct _TNode
{
	unsigned char code;
	int bitlen;
}TNode;

HNode hNode[33];
TNode hfmTb[17];
#if 0
#define PRINT_HFM(t) do{  \
	printf("0x%02x ", t);  \
	for(j = 0;j<hfmTb[t].bitlen;j++)  \
	{  \
		printf("%01d", (hfmTb[t].code >> (7 - j)) & 0x01);  \
	}  \
	printf(" %2d\n", hfmTb[t].bitlen);  \
}while(0)
#else
#define PRINT_HFM(t)
#endif
void goTree( int i, int len, unsigned char *a)
{
	
	if( i == -1 )
		return;
	if( hNode[i].left == -1 && hNode[i].right == -1)
	{
		//printf("%d %d ", i, len);
		memcpy(&hfmTb[i].code, a, sizeof(char));
		hfmTb[i].code &= (0xff00 >> len);
		hfmTb[i].bitlen = len;
		
		//printf("%02x\n", *a);

		//printf("\n");
		return;
	}
	if( hNode[i].left != -1)
	{
		(*a) &= ~(0x80 >> len);
		goTree(hNode[i].left, len+1, a);
	}
	if(hNode[i].right != -1)
	{
		(*a) |= (0x80 >> len);
		goTree(hNode[i].right, len+1, a);
	}
	
}

//哈夫曼压缩
void hfmPress( char * inFile, char * outFile )
{
	int i, j, m1, m2, k=0, max, h, l, len, blen;
	char b, c;
	unsigned char a;
	unsigned char buf[BUFSIZE];
	int counts[33] = {0};
	int height[33] = {0};
	int counts2[33]={0};
	FILE *f1, *f2;
	memset(hNode, 0, 33*sizeof(HNode));
	memset(hfmTb, 0, 17*sizeof(TNode));
	f1 = fopen(inFile, "rb");
	while(fread( &b, sizeof(char), 1, f1)!=0)
	{
		//printf("%02x ", b);
		c = (b & 0xf0) >> 4;
		counts[c]++;
		c = b & 0x0f;
		counts[c]++;
		
	}
	counts[16]=1;//结束符
	fclose(f1);
	printf("\n");
	for(i=0;i<33;i++)
	{
		printf("%d %d\n", i, counts[i]);
		hNode[i].val = counts[i];
		hNode[i].left = -1;
		hNode[i].right = -1;
		counts2[i]=counts[i];
	}
	printf("\n\n");
	for(k=17;k<33;k++)
	{
		
		m1 = -1; m2 = -1;
		max = 0x7fffffff;
		for(i=0;i<33;i++)
		{
			if(counts[i]!=0)
			{
				if(counts[i]<max)
				{
					m1 = i;
					max = counts[i];
				}
				else if(counts[i]==max && height[i]<height[m1])
				{
					m1 = i;
					max = counts[i];
				}
				
			}
			
		}
		max = 0x7fffffff;
		for(i=0;i<33;i++)
		{
			if(counts[i]!=0 && i!=m1)
			{
				if(counts[i]<max)
				{
					m2 = i;
					max = counts[i];
				}
				else if(counts[i]==max && height[i]<height[m2])
				{
					m2 = i;
					max = counts[i];
				}
			}
		}
		//printf("%d %d\n\n", m1, m2);
		hNode[k].val = counts[m1]+counts[m2];
		hNode[k].left = m1;
		hNode[k].right = m2;
		counts[k]=counts[m1]+counts[m2];
		height[k]=(height[m1]>height[m2]?height[m1]:height[m2])+1;
		counts[m1] = counts[m2] = 0;
		
	}
	printf("\n\n");
	for(i=0;i<33;i++)
	{
		printf("%2d %3d %3d\n", i, hNode[i].left, hNode[i].right);
	}
	printf("\n\n");
	h = height[32];
	memset(&hfmTb, 0xff, 16);
	a = 0;
	goTree(32, 0, &a);
	printf("\n\n");
	m1=0;
	for(i=0;i<17;i++)
	{
		printf("%2d ", i);
		//printf("%2d %2d %02x ", i, hfmTb[i].bitlen, hfmTb[i].code);
		for(j = 0;j<hfmTb[i].bitlen;j++)
		{
			printf("%01d", (hfmTb[i].code >> (7 - j)) & 0x01);
		}
		printf("\n");
		m1 += hfmTb[i].bitlen * counts2[i];
	}
	m1 += 17*16;
	m1=(m1+7)>>3;
	printf("ttsize: 0x%x B\n", m1);
	f1 = fopen(inFile, "rb");
	f2 = fopen(outFile, "wb");
	
	for(i=0;i<17;i++)
	{
		
		c = (char)(hfmTb[i].bitlen);
#if 1
		fwrite(&c, sizeof(char), 1, f2);
		fwrite(&(hfmTb[i].code), sizeof(char), 1, f2);
#endif
		printf("%02x %02x\n",c, hfmTb[i].code);
	}	
	
	blen = 0;
	len = 0;
	memset(buf, 0, BUFSIZE);
	while(fread( &b, sizeof(char), 1, f1)!=0)
	{

		c = (b & 0xf0) >> 4;
		l = hfmTb[c].bitlen;
		
		PRINT_HFM(c);
		
		if(blen + l <= 8)
		{
			buf[len] |= hfmTb[c].code >> blen;
			blen += l;
			if(blen == 8)
			{
				blen = 0;
				len++;
				if(len >= BUFSIZE )
				{
					fwrite( buf, sizeof(char), BUFSIZE, f2 );
					memset(buf, 0, BUFSIZE);
					len=0;			
				}
			}
		}else{
			buf[len] |= hfmTb[c].code >> blen;
			blen = blen + l - 8;
			len++;
			if(len >= BUFSIZE )
			{
				fwrite( buf, sizeof(char), BUFSIZE, f2 );
				memset(buf, 0, BUFSIZE);
				len=0;			
			}
			buf[len] |= hfmTb[c].code << l - blen;
		}
		
		c = b & 0x0f;
		l = hfmTb[c].bitlen;
		
		PRINT_HFM(c);
		
		if(blen + l <= 8)
		{
			buf[len] |= hfmTb[c].code >> blen;
			blen += l;
			if(blen == 8)
			{
				blen = 0;
				len++;
				if(len >= BUFSIZE )
				{
					fwrite( buf, sizeof(char), BUFSIZE, f2 );
					memset(buf, 0, BUFSIZE);
					len=0;			
				}
			}
		}else{
			buf[len] |= hfmTb[c].code >> blen;
			blen = blen + l - 8;
			len++;
			if(len >= BUFSIZE )
			{
				fwrite( buf, sizeof(char), BUFSIZE, f2 );
				memset(buf, 0, BUFSIZE);
				len=0;			
			}
			buf[len] |= hfmTb[c].code << l - blen;
		}
			
	}
	c = 16;
	l = hfmTb[c].bitlen;
	
	PRINT_HFM(c);
	
	if(blen + l <= 8)
	{
		buf[len] |= hfmTb[c].code >> blen;
		blen += l;
		if(blen == 8)
		{
			blen = 0;
			len++;
			if(len >= BUFSIZE )
			{
				fwrite( buf, sizeof(char), BUFSIZE, f2 );
				memset(buf, 0, BUFSIZE);
				len=0;			
			}
		}
	}else{
		buf[len] |= hfmTb[c].code >> blen;
		blen = blen + l - 8;
		len++;
		if(len >= BUFSIZE )
		{
			fwrite( buf, sizeof(char), BUFSIZE, f2 );
			memset(buf, 0, BUFSIZE);
			len=0;			
		}
		buf[len] |= hfmTb[c].code << l - blen;;
	}
	if(blen>0)
	{
		blen = 0;
		len++;
	}
	if(len > 0)
	{
		fwrite( buf, sizeof(char), len, f2 );
	}
	fclose(f1);
	fclose(f2);
}
//游程压缩 效果奇差 抛弃
void goRunPress( char * inFile, char * outFile )
{
	int i, j, k=0;
	char b, c;
	FILE *f1, *f2;
	//Run Length Press
	f1 = fopen(inFile, "rb");
	f2 = fopen(outFile, "wb");
	
	i = fread( &c, sizeof(char), 1, f1);	
	b=c;
	j = 0xffffffff;
	
	while(i!=0)
	{

		k = 1;
		
		while((i=fread( &c, sizeof(char), 1, f1))!=0 && b==c && k < 0xff)  
		{
			k++;
		}
		if(j == 0xffffffff)
		{
			if(k == 1)
			{
				j = 0;
				fwrite( &j, sizeof(char), 2, f2 );
			}else{
				j = 0x0101;
				fwrite( &j, sizeof(char), 2, f2 );
			}
		}else if(k==1 && j==0)
		{
			fwrite( &j, sizeof(char), 2, f2 );
		}else if(k>1 && j==0x0101){
			fwrite( &j, sizeof(char), 2, f2 );
		}
		
		if(k == 1)
		{
			j = 0x0101;
			fwrite( &b, sizeof(char), 1, f2 );
		}else{
			j = 0;
			fwrite( &b, sizeof(char), 1, f2 );
			fwrite( &k, sizeof(char), 1, f2 );
		}

		b = c;
	}
	
	fclose(f1);
	fclose(f2);
	
}
int main( void )
{
	hfmPress(FILENAME,"Press1.dat");
	//goRunPress("Press1.dat","Press2.dat");	
	return 0;
}

