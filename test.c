#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "searchEngine.h"

int main()
{
	char	c, fname[NAME_LENGTH];
	char	key[WORD_LENGTH];
	int	dcount=0;

	printf("***************command***************\n");
	printf("R: Input&Read Data,   S: Search Word\n");
	printf("I: Indexed Result,  Q: Quit\n");
	printf("*************************************\n");

        strcpy(document_name[dcount], "./txt/doc023.txt");
		bulid_hash_table("./txt/doc023.txt", dcount);
	
}
int transform(char *key) {
	int n = 0;
	while (*key!='\0')
		n += *key++;
	return n;
}
void search_word(char *key, int d) {
	int i;
	treePtr result;

	for (i = 0; i < d; i++)
		{
			treePtr p = hash_table[i][hash(key)];
			result = search_bst(key, p);
			print_result(result, i);
			if(result!=NULL)
				print_doc(document_name[i], result);
		}
	printf("\ntotal number of comparison = %d\n", num_search);
	num_search = 0;
}
void make_bst(char *key, treePtr p, treePtr temp)
{
	indexed_word++;
	treePtr a;
	a = p;
	while (a) {
		num_comparison++;
		if (strcmp(key, a->data) == 0)
		{
			a->count++;
			return;
		}
		p = a;
		if (strcmp(key, a->data) < 0) 
			a = a->left;
		else                 
			a = a->right;
	}

	if (p != NULL)
	{
		if (strcmp(key, p->data) < 0)
			p->left = temp;
		else
			p->right = temp;
	}
}
treePtr search_bst(char *key, treePtr p)
{
	treePtr ptr = p;
	while (ptr != NULL) {
		num_search++;
		if (strcmp(key, ptr->data) == 0) 
			return ptr;
		if (strcmp(key, ptr->data) < 0) 
			ptr = ptr->left;
		else                             
			ptr = ptr->right;
	}
	return NULL;
}
void bulid_hash_table(char *fname, int d)
{
	char line[2000];
	char *token;

	FILE *fp;
	fp = fopen(fname, "r");
	if (fp==NULL)
	{
		printf("There is no file.\n");
		return;
	}
	else {
		while (fgets(line, 2000, fp) != NULL)
		{
			token = strtok(line, delimiter);
			while (token != NULL)
			{
				insert_hash_table(token, d);
				token = strtok(NULL, delimiter);
			}
		}
		printf("<%s> File indexig is completed.\n",fname);
	}
	fclose(fp);
}
void insert_hash_table(char *key, int d)
{
	treePtr temp = (treePtr)malloc(sizeof(node));
	strncpy(temp->data, key, WORD_LENGTH - 1); 
	temp->count = 1;
	temp->left = temp->right = NULL;

	treePtr q = hash_table[d][hash(key)];
	if (q == NULL)
	{
		num_comparison++;
		indexed_word++;
		hash_table[d][hash(key)]= temp;
	}
	else
		make_bst(key, q, temp);
}

int hash(char *key) {
	return transform(key) % 30;
}
void print_idx_result(int i)
{
	printf("\nTotal number of documents = %d \n", i);
	printf("Total number of indexed words = %d \n", indexed_word);
	printf("Total number of comparison = %d\n", num_comparison);
}
void print_result(treePtr p, int i) 
{	
	if (p) {
		printf("\n<%s> %s : %d", document_name[i], p->data, p->count);
	}
}
void print_doc(char *fname, treePtr p)
{
	char line[2000];
	char printLine[2000];
	char *token, *token2;

	FILE *fp;
	fp = fopen(fname, "r");
	if (fp == NULL)
	{
		printf("There is no file.\n");
		return;
	}
	while (fgets(line, 2000, fp) != NULL)
	{
		strcpy(printLine, line);
		token = strtok(line, delimiter);
		while (token != NULL)
		{
			if (strcmp(token, p->data) == 0) { 
				printf("\n * %s", printLine);
			}
			
			token = strtok(NULL, delimiter);
		}
	}
	printf("\n");
	fclose(fp);
}