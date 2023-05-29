#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define FILENAME "Press1.dat"
#define HFM_SIZE (17)
#define NODE_SIZE (33)

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

HNode hNode[NODE_SIZE];
TNode hfmTb[HFM_SIZE];

#if 0
int myCpr( TNode *tn1, TNode *tn2 )
{
	int i;
	char b1, b2;
	char c1=tn1->code;
	char c2=tn2->code;
	int i1 = tn1->bitlen;
	int i2 = tn1->bitlen;
	for(i = 7;i>=0;i--)
	{
		b1 = c1>>i&0x01;
		b2 = c2>>i&0x01;
		if(b1>b2)
		{
			return 1;
		}
		if(b1<b2)
		{
			return -1;
		} 
	}
	return 0; 
}

void mySort(TNode * hfmTb)
{
	int i, j;
	char c;
	int bl; 
	TNode tn;
	for(i=1;i<HFM_SIZE;i++)
	{
		tn.code = hfmTb[i].code;
		tn.bitlen = hfmTb[i].bitlen;
		for(j=i-1;j>=0&&myCpr(&tn, &hfmTb[j])<0;j--)
		{
			hfmTb[j+1].code = hfmTb[j].code;
			hfmTb[j+1].bitlen = hfmTb[j].bitlen; 
		}
		hfmTb[j+1].code = tn.code;
		hfmTb[j+1].bitlen = tn.bitlen;
	}
	return;
}
#endif

void reBuildTree( TNode * hfmTb, HNode * hNode )
{
	int i, j, k;
	HNode *curr = NULL;
//	for(i=0;i<HFM_SIZE;i++)
//	{
//		hNode->val = i;
//		hNode->left = -1;
//		hNode->right = -1;
//	}
	
	k = NODE_SIZE - 1;
	k--;
	for(i=0;i<HFM_SIZE;i++)
	{
		curr = &hNode[NODE_SIZE - 1];
		
		for(j=7;j>=8-hfmTb[i].bitlen;j--)
		{
			
			if((hfmTb[i].code >> j & 0x01) == 0)
			{
				
				if(curr->left == -1)
				{
					
					curr->left = k;
					curr = &hNode[k];
					k--;
				}else{
					
					curr = &hNode[curr->left];
				}		
			}else{
				
				if(curr->right == -1)
				{
					
					curr->right = k;
					curr = &hNode[k];
					k--;
				}else{
					
					curr = &hNode[curr->right];
				}
			}			
		}
		curr->val = i;
	}
} 

//哈夫曼解压缩
int hfmUnPress( char * inFile, char * outFile )
{
	int i, j, k;
	unsigned char a, b, c;
	HNode *curr = NULL;
	FILE *f1, *f2;
	memset(hNode, 0xff, NODE_SIZE*sizeof(HNode));
	memset(hfmTb, 0xff, HFM_SIZE*sizeof(TNode));
	f1 = fopen(inFile, "rb");
	for(i=0;i<17;i++)
	{
		if(fread(&b, sizeof(char), 1, f1)!=0)
		{
			hfmTb[i].bitlen = b;
		}else return 1; 
		if(fread(&b, sizeof(char), 1, f1)!=0)
		{
			hfmTb[i].code = b;
		}else return 1;
	}
	for(i=0;i<17;i++)
	{
		printf("%2d ", i);
		for(j = 0;j<hfmTb[i].bitlen;j++)
		{
			printf("%01d", (hfmTb[i].code >> (7 - j)) & 0x01);
		}
		printf("\n");
	}
	//mySort((TNode *)hfmTb);
	reBuildTree((TNode *)hfmTb, (HNode *)hNode);
	printf("\n\n");
	for(i=0;i<33;i++)
	{
		printf("%2d %2d %3d %3d\n", i, hNode[i].val, hNode[i].left, hNode[i].right);
	}
	
	f2 = fopen(outFile, "wb");
	
	c = 0;
	j = 0;
	curr = &hNode[NODE_SIZE - 1];
	while(fread( &b, sizeof(char), 1, f1)!=0)
	{
		for(i=7;i>=0;i--)
		{
			
			if((b >> i & 0x01) == 0)
			{
				curr = &hNode[curr->left];
			}else{
				curr = &hNode[curr->right];
			}
			if(curr->val != -1)
			{
				if(curr->val == 0x10)
					break;
				if(j == 0)
				{
					c = curr->val << 4 & 0xf0;
				}else{
					c |= curr->val & 0x0f;
					fwrite( &c, sizeof(char), 1, f2 );
				}			
				j = 1-j;
				curr = &hNode[NODE_SIZE - 1];
			}
		}			
	}
	fclose(f1);
	fclose(f2);	
	if(curr->val != 0x10)
		return 1;
	return 0;
}

int main(void)
{
	int f = hfmUnPress( FILENAME, "UnPress1.bin");
	printf(f==0?"Suc":"Fail");
	return 0;
}

