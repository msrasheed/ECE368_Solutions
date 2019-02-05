#include "huffman.h"

long * countChars(FILE * in)
{
	long * counts = malloc(sizeof(long) * NUM_ASCII_CHARS);
	int c = fgetc(in);
	while (c >= 0)
	{
		counts[c]++;
		c = fgetc(in);
	}
	return counts;
}

Listnode * createListnode(Treenode * tn)
{
	Listnode * nd = malloc(sizeof(Listnode));
	nd->next = NULL;
	nd->tnptr = tn;
	return nd;
}

Listnode ** insertNode(Listnode ** hdtl, Listnode * nd)
{
	//fprintf(stdout, "inserting %d %ld\n", nd->tnptr->val, nd->tnptr->freq);
	Listnode * head = hdtl[0];
	Listnode * tail = hdtl[1];
	tail->next = nd;
	Listnode * curr = head;
	Listnode * prev = NULL;
	while (curr->tnptr->freq < nd->tnptr->freq)
	{
		prev = curr;
		curr = curr->next;
	}
	while (curr->tnptr->val < nd->tnptr->val && curr->tnptr->freq == nd->tnptr->freq)
	{
		prev = curr;
		curr = curr->next;
	}
	while (curr->tnptr->val == nd->tnptr->val && curr->tnptr->freq == nd->tnptr->freq && curr != nd)
	{
		prev = curr;
		curr = curr->next;
	}
	if (curr == nd)
	{
		hdtl[1] = nd;
		//printCnts(hdtl);
		return hdtl;
	}
	else if (curr == head)
	{
		nd->next = head;
		hdtl[0] = nd;
		tail->next = NULL;
		return hdtl;
	}
	else 
	{
		prev->next = nd;
		nd->next = curr;
		tail->next = NULL;
		//printCnts(hdtl);
		return hdtl;
	}
}

Listnode ** createList(long * cnts)
{
	Listnode ** hdtl = malloc(sizeof(Listnode*) * 2);
	int i;
	for(i = 0; i < NUM_ASCII_CHARS; i++)
	{
		if(cnts[i] != 0)
		{
			Treenode * tn = createTreenode(i, cnts[i]);
			hdtl[0] = createListnode(tn);
			hdtl[1] = hdtl[0];
			//fprintf(stdout, "%c, %ld\n", i, cnts[i]);
			break;
		}
	}
	int j;
	for (j = i+1; j < NUM_ASCII_CHARS; j++)
	{
		if (cnts[j] != 0)
		{
			Treenode * tn = createTreenode(j, cnts[j]);
			Listnode * ln = createListnode(tn);
			hdtl = insertNode(hdtl, ln);
			//fprintf(stdout, "%c, %ld\n", j, cnts[j]);
			//printCnts(hdtl);
			//fprintf(stdout, "\n\n");
		}
	}
	return hdtl;
}

Listnode ** mergeNodes(Listnode ** hdtl)
{
	Treenode * tn = mergeTreenodes(hdtl[0]->tnptr, hdtl[0]->next->tnptr);
	Listnode * ln = createListnode(tn);
	Listnode * nh = hdtl[0]->next->next;
	free(hdtl[0]->next);
	free(hdtl[0]);
	hdtl[0] = nh;
	if (hdtl[0] != NULL)
	{
		return insertNode(hdtl, ln);	
	}
	else
	{
		hdtl[0] = ln;
		hdtl[1] = hdtl[0];
		return hdtl;
	}
}

Treenode * createTreenode(int value, long frequency)
{
	Treenode * tn = malloc(sizeof(Treenode));
	tn->left = NULL;
	tn->right = NULL;
	tn->val = value;
	tn->freq = frequency;
	return tn;
}

Treenode * mergeTreenodes(Treenode * left, Treenode * right)
{
	Treenode * tn = createTreenode(NON_NODE_VAL, left->freq + right->freq);
	tn->left = left;
	tn->right = right;
	return tn;
}

Treenode * createTree(Listnode ** hdtl)
{
	while(hdtl[0]->next != NULL)
	{
		hdtl = mergeNodes(hdtl);
		//printCnts(hdtl);
		//fprintf(stdout, "\n\n");
	}
	Treenode * tn = hdtl[0]->tnptr;
	free(hdtl[0]);
	return tn;
}

void preOrderTraversal(Treenode * tn, FILE * in)
{
	if (tn == NULL)
	{
		return;
	}
	if(tn->val == NON_NODE_VAL)
	{
		fprintf(in, "0");
	}
	else
	{
		fprintf(in, "1%c", tn->val);
	}
	preOrderTraversal(tn->left, in);
	preOrderTraversal(tn->right, in);
}

int treeHeight(Treenode * tn, int count)
{
	if (tn == NULL)
	{
		return count - 1;
	}
	int cnt1 = treeHeight(tn->left, count + 1);
	int cnt2 = treeHeight(tn->right, count + 1);
	if (cnt1 > cnt2)
	{
		return cnt1;
	}
	else
	{
		return cnt2;
	}
}

void treeNumLeafs(Treenode * tn, int * count)
{
	if (tn->left == NULL)
	{
		*count = *count + 1;
		return;
	}
	treeNumLeafs(tn->left, count);
	treeNumLeafs(tn->right, count);
}

void destroyTree(Treenode * tn)
{
	if (tn == NULL)
	{
		return;
	}
	destroyTree(tn->left);
	destroyTree(tn->right);
	free(tn);
}

void createCodeBook(int ** codebook, int * letToCode, int * temp, Treenode * tree, FILE * out)
{
	int codeInd = 0;
	createCodeBookHelper(codebook, letToCode, temp, tree->left, out, &codeInd, 0, 0);
	createCodeBookHelper(codebook, letToCode, temp, tree->right, out, &codeInd, 0, 1);
}

void createCodeBookHelper(int ** codebook, int * letToCode, int * temp, Treenode * tree, FILE * out, int * codeInd, int tempInd, int val)
{
	temp[tempInd] = val;
	if (tree->left == NULL)
	{
		codebook[*codeInd] = malloc(sizeof(int) * (tempInd + 2));
		int i;
		for (i = 0; i < tempInd + 1; i++)
		{
			codebook[*codeInd][i] = temp[i];
		}
		codebook[*codeInd][i] = -1;
		letToCode[tree->val] = *codeInd;

		printCode(temp, tempInd + 1, tree->val, out);
		*codeInd = *codeInd + 1;
		return;
	}
	createCodeBookHelper(codebook, letToCode, temp, tree->left, out, codeInd, tempInd + 1, 0);
	createCodeBookHelper(codebook, letToCode, temp, tree->right, out, codeInd, tempInd + 1, 1);
}

void printCode(int * code, int n, char val, FILE * out)
{
	int i;
	fprintf(out, "%c:", val);
	for (i = 0; i < n; i++)
	{
		fprintf(out, "%d", code[i]);
	}
	fprintf(out, "\n");
}

void writeBit(FILE * out, char val, char * byte, char * byteLoc)
{
	*byte = *byte | (val << *byteLoc);
	*byteLoc = *byteLoc + 1;
	if (*byteLoc > 7)
	{
		fwrite(byte, 1, sizeof(char), out);
		*byteLoc = 0;
		*byte = 0;
	}
}

void writeChar(FILE * out, char val, char * byte, char * byteLoc)
{
	int i;
	for(i = 0; i < 8; i++)
	{
		writeBit(out, (val >> i) & 0x1, byte, byteLoc);
	}
}

void writeTree(FILE * out, Treenode * tree, char * byte, char * byteLoc)
{
	if (tree->val == NON_NODE_VAL)
	{
		writeBit(out, 0x0, byte, byteLoc);
		writeTree(out, tree->left, byte, byteLoc);
		writeTree(out, tree->right, byte, byteLoc);
		return;
	}
	else 
	{
		writeBit(out, 0x1, byte, byteLoc);
		writeChar(out, tree->val, byte, byteLoc);
	}
}

void writeCode(FILE * out, int * code, char * byte, char * byteLoc)
{
	int i;
	for(i = 0; code[i] > -1; i++)
	{
		writeBit(out, code[i], byte, byteLoc);
	}
}

void encodeFile(FILE * in, FILE * out, Treenode * tree, int * letToCode, int ** codebook, long numChars)
{
	long numCharsComp = 0;
	long charTree = 0;
	fwrite(&numCharsComp, 1, sizeof(long), out);
	fwrite(&charTree, 1, sizeof(long), out);
	fwrite(&numChars, 1, sizeof(long), out);

	char byte = 0;
	char byteLoc = 0;
	if (tree != NULL)
	{
		writeTree(out, tree, &byte, &byteLoc);
		while (byteLoc != 0)
		{
			writeBit(out, 0x0, &byte, &byteLoc);
		}
	}
	charTree = ftell(out) - sizeof(long) * 3;

	int c = fgetc(in);
	while (c >= 0)
	{
		writeCode(out, codebook[letToCode[c]], &byte, &byteLoc);
		c = fgetc(in);
	}
	while (byteLoc != 0)
	{
		writeBit(out, 0x0, &byte, &byteLoc);
	}

	numCharsComp = ftell(out);
	fseek(out, 0, SEEK_SET);
	fwrite(&numCharsComp, 1, sizeof(long), out);
	fwrite(&charTree, 1, sizeof(long), out);
}

void printCnts(Listnode ** hdtl)
{
	Listnode * head = hdtl[0];
	while (head != NULL)
	{
		fprintf(stdout, "%c, %ld \n", head->tnptr->val, head->tnptr->freq);
		head = head->next;
	}
}

void printCodeBook(int ** codebook, int *letToCode)
{
	int i;
	for(i = 0; i < NUM_ASCII_CHARS; i++)
	{
		if(letToCode[i] != -1)
		{
			fprintf(stdout, "%c: ", i);
			int j;
			for(j = 0; codebook[letToCode[i]][j] > -1; j++)
			{
				fprintf(stdout, "%d", codebook[letToCode[i]][j]);
			}
			fprintf(stdout, "\n");
		}
	}
}