#ifndef COMMONSTRUCT_H
#define COMMONSTRUCT_H

#include <stdlib.h>
#include <stdint.h>

typedef struct __TreeNode_S
{
    void * mParentP;
    void * mLeftChildP;
    void * mRightChildP;
    uint8_t mVirtualFlag;      //虚拟标志
    union
    {
        uint32_t mUint32Value;
        void * mVoidPointP;
    }mValue_U;
}TreeNode_S;

#define INIT_TREENODE( A )  do{ \
( ( TreeNode_S * )( A ) )->mLeftChildP = NULL;  \
    ( ( TreeNode_S * )( A ) )->mRightChildP = NULL;  \
    ( ( TreeNode_S * )( A ) )->mParentP = NULL;  \
    ( ( TreeNode_S * )( A ) )->mValue_U.mVoidPointP = NULL;  \
    ( ( TreeNode_S * )( A ) )->mVirtualFlag = 0;  \
}while( 0 );


typedef struct __ListElemet_S
{
    void * NextP;
    void * PrevP;
    void * ElementP;
}ListElement_S;

#define INIT_LISTELEMENT( A )   do{ \
                                    ( ( ListElement_S * )( A ) )->NextP = NULL; \
                                    ( ( ListElement_S * )( A ) )->PrevP = NULL; \
                                    ( ( ListElement_S * )( A ) )->ElementP = NULL; \
                                }while( 0 )


typedef struct __Stack_S
{
    ListElement_S * stackElementp;
    uint32_t ElementCount;
}Stack_S;

#define INIT_STACK( A )     do{ \
                                ( ( Stack_S * )( A ) )->stackElementp = NULL;   \
                                ( ( Stack_S * )( A ) )->ElementCount = 0;   \
                            }while( 0 )

#define PUSH_STACK( A, B )  do{ \
                                if( ( ( Stack_S * )A )->stackElementp != NULL ) {   \
                                    ( ( Stack_S * )A )->stackElementp->NextP = ( ListElement_S * )B;    \
                                    ( ( ListElement_S * )B )->PrevP = ( ( Stack_S * )A )->stackElementp;    \
                                    ( ( Stack_S * )A )->stackElementp = ( ListElement_S * )B;   \
                                } else {   \
                                    ( ( Stack_S * )A )->stackElementp = ( ListElement_S * )B;   \
                                }   \
                                ( ( Stack_S * )A )->ElementCount += 1;  \
                            }while( 0 )

#define POP_STACK( A, RET ) do{ \
                                if( ( ( Stack_S * )A )->ElementCount == NULL ) {    \
                                    RET = NULL; \
                                } else {    \
                                    RET = ( ( Stack_S * )A )->stackElementp;   \
                                    ( ( Stack_S * )A )->stackElementp = ( ( Stack_S * )A )->stackElementp->PrevP; \
                                    ( ( Stack_S * )A )->ElementCount -= 1;    \
                                }   \
                            }while( 0 )

#endif // COMMONSTRUCT_H
