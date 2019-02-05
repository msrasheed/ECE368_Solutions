#include <stdio.h>
#include <stdlib.h>

#define NUM_ASCII_CHARS 256
#define NON_NODE_VAL 300

typedef struct treenode
{
	struct treenode * left;
	struct treenode * right;
	int val;
	long freq;
}Treenode;

typedef struct listnode
{
	struct listnode * next;
	Treenode * tnptr;
}Listnode;

long * countChars(FILE * in);

Listnode * createListnode(Treenode * tn);
Listnode ** insertNode(Listnode ** hdtl, Listnode * nd);
Listnode ** createList(long * cnts);
Listnode ** mergeNodes(Listnode ** hdtl);

Treenode * createTreenode(int value, long frequency);
Treenode * mergeTreenodes(Treenode * left, Treenode * right);
Treenode * createTree(Listnode ** hdtl);
void preOrderTraversal(Treenode * tn, FILE * in);
int treeHeight(Treenode * tn, int count);
void treeNumLeafs(Treenode * tn, int * count);
void destroyTree(Treenode * tn);

void createCodeBook(int ** codebook, int * letToCode, int * temp, Treenode * tree, FILE * out);
void createCodeBookHelper(int ** codebook, int * letToCode, int * temp, Treenode * tree, FILE * out, int * codeInd, int tempInd, int val);
void printCode(int * code, int n, char val, FILE * out);

void writeBit(FILE * out, char val, char * byte, char * byteLoc);
void writeChar(FILE * out, char val, char * byte, char * byteLoc);
void writeTree(FILE * out, Treenode * tree, char * byte, char * byteLoc);
void writeCode(FILE * out, int * code, char * byte, char * byteLoc);
void encodeFile(FILE * in, FILE * out, Treenode * tree, int * letToCode, int ** codebook, long numChars);

void printCnts(Listnode** hdtl);
void printCodeBook(int ** codebook, int *letToCode);