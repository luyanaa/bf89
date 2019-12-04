#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* bf89 - A Simple BrainF**k Interpreter In Pure C89 */

FILE *FSrcPointer,*FLogPointer,*FTempPointer;

typedef ErrType unsigned char;
typedef IntType unsigned long;
typedef SignedIntType long;

#define MAXBLOCK 100
IntType BlockCount=0;
SignedIntType BlockLeft[MAXBLOCK],BlockRight[MAXBLOCK],LeftMost,RightMost;
typedef struct MemBlock{
	IntType BlockNum;
	char Buf[1000];
	SignedIntType L,R;
}MemBlock;
MemBlock *UsableBlock;

SignedIntType MemPointer=0,Offset;
char OpBuf;
fpos_t RepeatStartPos;

#define 
void BackupBlock() {
	FILE *BlockTempFile;
	char *FILENAME=strcat("BF89",strcat(itoa(UsableBlock->BlockNum),".TMP"));
	BlockTempFile=fopen(FILENAME,"w");
	f
}
void ExchangeBlock(){
	IntType BlockI;
	unsigned char FindBlock=0;
	if (MemPointer < UsableBlock->L || MemPointer > UsableBlock->R) {
		BackupBlock();
		free(UsableBlock);
		for(BlockI=0;BlockI<BlockCount;++BlockI) {
			if(MemPointer<=BlockRight[BlockI]&&MemPointer>=BlockLeft[BlockI]) {
				FindBlock==1;
				break;
			}
		}
		if(FindBlock) ReadBlock(BlockI);
		else {
			if(BlockI<LeftMost) NewBlock(++BlockCount,LeftMost-1000,LeftMost-1);
			if(BlockI>RightMost) NewBlock(++BlockCount,RightMost+1,RightMost+1000);
		}
	}
	return ;
}

(char *) GetMemRef(SignedIntType Position){
	return &UsableBlock->Buf[Position-L];
}

void IncMemPointer(){
	++MemPointer;
	ExchangeBlock();
	return ;
}

void DecMemPointer(){
	--MemPointer;
	ExchangeBlock();
	return ;
}

void IncMemValue(){
	++(*(GetMemRef(MemPointer)));
	return ;
}

void DecMemValue(){
	--(*(GetMemRef(MemPointer)));
	return ;
}

void OutPutMemValue(){
	putchar(*(GetMemRef(MemPointer)));
	return ;
}

void InputMemValue(){
	*(GetMemRef(MemPointer))=getchar();
	return ;
}

void JumpForward(){
	
}

void JumpBackward(){
	
}

typedef unsigned char err;
void ThrowError(err errvalue){
	if(errvalue==InvaildOperation) {
		
	}
	
	exit(errvalue);
}
int main(int argc,char **argv){
	fprintf("bf89 - A Simple BrainF**k Interpreter In Pure C89\n");
	if(argc != 1) {
		fprintf(stderr,"Usage: bf <Source Code>\n");
		exit(-1);
	}
	
	FSrcPointer=fopen(argv[0],"r");
	
	if(FSrcPointer==(FILE *)NULL) {
		fprintf(stderr,"Error when open file %s\n",argv[0]);
		exit(-1);
	}
	
	UsableBlock=(MemBlock *)malloc(sizeof(MemBlock));
	memset(UsableBlock,0,sizeof(UsableBlock));

	while(!feof(FSrcPointer)) {
		fget(FSrcPointer,&OpBuf);
		switch (FSrcPointer) {
			case '>' :
				IncMemPointer();
				break;
			case '<' :
				DecMemPointer();
				break;
			case '+' :
				IncMemValue();
				break;
			case '-' :
				DecMemValue();
				break;
			case '.' :
				InputMemValue();
				break;
			case ',' :
				OutputMemValue();
				break;
			case '[' :
				JumpForward();
				break;
			case ']' :
				JumpBackward();
				break;
			default :
				ThrowError(InvaildOperation);
				break;
		}
	}

	DeleteTempFile();	
	return 0;
}

