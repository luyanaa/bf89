#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
/* bf89 - A Simple BrainF**k Interpreter In Pure C89 */

FILE *FSrcPointer,*FLogPointer,*FTempPointer;

typedef unsigned char ErrType;
typedef unsigned long IntType;
typedef long SignedIntType;
#define SMIN LONG_MIN
#define SMAX LONG_MAX
#define UMAX ULONG_MAX
ErrType UnpairedOp=-2,InvaildOperation=-3;
#define MAXBLOCK 5000
#define BLOCKSIZE 30000

IntType BlockCount=0;
IntType BlockLeft[MAXBLOCK],BlockRight[MAXBLOCK],LeftMost=SMAX,RightMost=0;

typedef struct MemBlock{
	IntType BlockNum;
	char Buf[BLOCKSIZE];
	IntType L,R;
}MemBlock;
MemBlock *UsableBlock;

IntType MemPointer=0;
SignedIntType Offset;
char OpBuf;

IntType min(IntType L,IntType R){
	return L<R?L:R;
}
IntType max(IntType L,IntType R){
	return L>R?L:R;
}

void ThrowError(ErrType ErrValue){
        if(ErrValue==InvaildOperation) {
                fprintf(stderr,"Invaild Operation, please check you source code.\n");
        }
        if(ErrValue==UnpairedOp) {
                fprintf(stderr,"There exists unpaired [], please check your source code.\n");
        }
        exit(ErrValue);
}

char *itoa(IntType Value){
	char *Buf=(char *)malloc(sizeof(char)*10),FLAG=0;
	IntType Length=0,i;
	memset(Buf,0,sizeof(char)*10);
	if(Value==0) { Buf[0]='0';Buf[1]='\0'; return Buf;}
	while(Value){
		Buf[Length++]=Value%10+'0';
		Value/=10;
	}
	for(i=0;i<Length/2;++i) Buf[i]^=Buf[Length-i-1],Buf[Length-i-1]^=Buf[i],Buf[i]^=Buf[Length-i-1];
	return Buf;
}
	
/* void BackupBlock() */
void BackupBlock() {
	FILE *BlockTempFile;
	char *FILENAME=(char *)malloc(sizeof(char)*10);
	char *nStrTmp;
	strcpy(FILENAME,"BF89");
	strcat(FILENAME,strcat(itoa(UsableBlock->BlockNum),".TMP"));
	size_t Output;
	BlockTempFile=fopen(FILENAME,"w");
	nStrTmp=itoa(UsableBlock->BlockNum);
	fprintf(BlockTempFile,"%s,",nStrTmp);
	free(nStrTmp);
	for(Output=0;Output<BLOCKSIZE;++Output){
		nStrTmp=itoa(UsableBlock->Buf[Output]);
		fprintf(BlockTempFile,"%s,",nStrTmp);
		free(nStrTmp);
	}
	nStrTmp=itoa(UsableBlock->L);
	fprintf(BlockTempFile,"%s,",nStrTmp);
	free(nStrTmp);
	nStrTmp=itoa(UsableBlock->R);
	fprintf(BlockTempFile,"%s,",nStrTmp);
	free(nStrTmp);
	fclose(BlockTempFile);
	free(FILENAME);
	return ;
}

IntType ReadValue(FILE *fp){
	char *Buffer=(char *)malloc(sizeof(char)*10);
	size_t EndPos=0;
	SignedIntType RetValue;
	memset(Buffer,0,sizeof(char)*10);
	while((Buffer[EndPos]=fgetc(fp))!=EOF&&Buffer[EndPos]!=',') ++EndPos;
	Buffer[EndPos]='\0';
	RetValue=atoi(Buffer);
	free(Buffer);
	return RetValue;
}

/* void ReadBlock(IntType) */
void ReadBlock(IntType BlockNum) {
	size_t i;
	FILE *BlockTempFile;
	char *FILENAME=(char *)malloc(sizeof(char)*10);
        strcpy(FILENAME,"BF89");
        strcat(FILENAME,strcat(itoa(UsableBlock->BlockNum),".TMP"));
	BlockTempFile=fopen(FILENAME,"r");
	UsableBlock=(MemBlock *)malloc(sizeof(MemBlock));
	UsableBlock->BlockNum=ReadValue(BlockTempFile);
	for(i=0;i<BLOCKSIZE;++i) {
		UsableBlock->Buf[i]=(char)ReadValue(BlockTempFile);
	}
	UsableBlock->L=ReadValue(BlockTempFile);
	UsableBlock->R=ReadValue(BlockTempFile);

	fclose(BlockTempFile);	
	return ;
}

/* void NewBlock (IntType,IntType,IntType) */
void NewBlock(IntType BlockNum,IntType L,IntType R){
	LeftMost=min(L,LeftMost);
	RightMost=max(R,RightMost);
	UsableBlock=(MemBlock *)malloc(sizeof(MemBlock));
	memset(UsableBlock,0,sizeof(MemBlock));
	UsableBlock->BlockNum=BlockNum;
	UsableBlock->L=L,UsableBlock->R=R;
	return ;
}

/* void ExchangeBlock() */
void ExchangeBlock(){
	IntType BlockI;
	unsigned char FindBlock=0;
	if (MemPointer < UsableBlock->L || MemPointer > UsableBlock->R) {
		#ifdef DEBUG
			printf("Block has been exchanged\n");
		#endif	
		/* Make a backup of the block in the memory and delete it from memory. */
		BackupBlock();
		free(UsableBlock);
		
		/* Find if there exists the block we need. */
		for(BlockI=0;BlockI<BlockCount;++BlockI) {
			if(MemPointer<=BlockRight[BlockI]&&MemPointer>=BlockLeft[BlockI]) {
				FindBlock=1;
				break;
			}
		}
		
		if(FindBlock) ReadBlock(BlockI);
		else {
			if(BlockI<LeftMost) NewBlock(++BlockCount,LeftMost-BLOCKSIZE,LeftMost-1);
			if(BlockI>RightMost) NewBlock(++BlockCount,RightMost+1,RightMost+BLOCKSIZE);
		}
	}
	return ;
}

char *GetMemRef(IntType Position){
	return &(UsableBlock->Buf[Position-UsableBlock->L]);
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
	*(GetMemRef(MemPointer))=getc(stdin);
	return ;
}

void JumpForward(){
	char Buf;
	size_t LeftNum=1,RightNum=0;
	if ( (*GetMemRef(MemPointer)) ==0 ) {
		while ((Buf=fgetc(FSrcPointer))&&!feof(FSrcPointer)) {
			if(Buf == ']') ++RightNum;
			if(Buf == '[') ++LeftNum;
			if(LeftNum == RightNum) break;
		}
		if ( feof(FSrcPointer) ) {
			ThrowError(UnpairedOp);
		}
	}
}

void JumpBackward(){
	char Buf;
	size_t RightNum=1,LeftNum=0;
	if ( (*GetMemRef(MemPointer)) != 0 ) {
		fseek(FSrcPointer,-1,SEEK_CUR);
		while (fseek(FSrcPointer,-1,SEEK_CUR)==0){
			Buf=fgetc(FSrcPointer);
			if(feof(FSrcPointer)) ThrowError(UnpairedOp);
			if(Buf=='[') ++LeftNum;
			if(Buf==']') ++RightNum;
			if(LeftNum == RightNum) break;
			if(fseek(FSrcPointer,-1,SEEK_CUR)) ThrowError(UnpairedOp);
		}
	}
}

int main(int argc,char **argv){
	if(argc != 2 && argc != 3 && argc!= 4) {
		fprintf(stderr,"bf89 - A Simple BrainF**k Interpreter In Pure C89\n");
		fprintf(stderr,"Usage: %s <Source Code> (<Input>) (<Output>) \n",argv[0]);
		exit(-1);
	}
	FSrcPointer=fopen(argv[1],"r");
	
	if(FSrcPointer==(FILE *)NULL) {
		fprintf(stderr,"Error when open file %s\n",argv[0]);
		exit(-1);
	}

	if(argc >= 3) 
		freopen(argv[2],"r",stdin);	
	
	if(argc == 4) 
		freopen(argv[3],"w",stdout);

	NewBlock(++BlockCount,0,BLOCKSIZE-1);

	while((OpBuf=fgetc(FSrcPointer)) && (!feof(FSrcPointer))) {
		/* Read Next Operation into OpBuf and decide what to do according to Brainf**k standard. */
		switch (OpBuf) {
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
				OutPutMemValue();
				break;
			case ',' :
				InputMemValue();
				break;
			case '[' :
				JumpForward();
				break;
			case ']' :
				JumpBackward();
				break;
			default :
				break;
		}
	}

	fclose(FSrcPointer);
	free(UsableBlock);
	fclose(stdin);
	fclose(stdout);
	return 0;
}

