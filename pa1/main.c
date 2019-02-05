#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

int main(int argc, char * * argv)
{
	if (argc < 6)
	{
		fprintf(stderr, "Not enough input arguments\n");
		return EXIT_FAILURE;
	}
	FILE * infptr = fopen(argv[1], "r");
	if (infptr == NULL)
	{
		fprintf(stderr, "Input file doesn't exist\n");
		return EXIT_FAILURE;
	}
	FILE * cntptr = fopen(argv[2], "w");
	if (infptr == NULL)
	{
		fprintf(stderr, "Count file doesn't exit\n");
		return EXIT_FAILURE;
	}
	long * cnts = countChars(infptr);
	fwrite(cnts, NUM_ASCII_CHARS, sizeof(long), cntptr);
	fclose(cntptr);


	Listnode ** hdtl = createList(cnts);
	free(cnts);
	Treenode * tree = NULL;
	if (hdtl[0] != NULL)
	{
		tree = createTree(hdtl);
	}
	FILE * treeptr = fopen(argv[3], "w");
	if (treeptr == NULL)
	{
		fprintf(stderr, "Tree file doesn't exit\n");
		return EXIT_FAILURE;
	}
	preOrderTraversal(tree, treeptr);
	fclose(treeptr);

	
	FILE * codeptr = fopen(argv[4], "w");
	if (codeptr == NULL)
	{
		fprintf(stderr, "Code file doesn't exist\n");
		return EXIT_FAILURE;
	}
	int height, numChars;
	int * letToCode = NULL;
	int * temp;
	int ** codebook;
	if (tree != NULL)
	{
		height = treeHeight(tree, 0);
		numChars = 0;
		treeNumLeafs(tree, &numChars);
		letToCode = malloc(sizeof(int) * NUM_ASCII_CHARS);
		/*int i;
		for (i = 0; i < NUM_ASCII_CHARS; i++)
		{
			letToCode[i] = -1;
		}*/
		codebook = malloc(sizeof(int *) * numChars);
		temp = malloc(sizeof(int) * height);
		createCodeBook(codebook, letToCode, temp, tree, codeptr);
		free(temp);
	}
	fclose(codeptr);
	//printCodeBook(codebook, letToCode);

	long numCharsOrig = ftell(infptr);
	fseek(infptr, 0, SEEK_SET);
	FILE * hbtptr = fopen(argv[5], "w");
	if (hbtptr == NULL)
	{
		fprintf(stderr, "Hbt file doesn't exist\n");
		return EXIT_FAILURE;
	}
	encodeFile(infptr, hbtptr, tree, letToCode, codebook, numCharsOrig);
	fclose(hbtptr);

	fclose(infptr);
	free(hdtl);
	if (tree != NULL)
	{
		int i;
		for(i = 0; i < numChars; i++)
		{
			free(codebook[i]);
		}
		destroyTree(tree);
		free(codebook);
		free(letToCode);
	}
	return EXIT_SUCCESS;
}