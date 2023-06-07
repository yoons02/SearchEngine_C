#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "searchEngine.h"

#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS

int main()
{
   char   c, fname[NAME_LENGTH];
   char   key[WORD_LENGTH];
   int   dcount = 0;//데이터를 읽어온 파일의 개수를 저장하는 변수이다.

   printf("***************command***************\n");
   printf("R: Input&Read Data,   S: Search Word\n");
   printf("I: Indexed Result,  Q: Quit\n");
   printf("*************************************\n");

   while (1) {
      printf("\nCommand> ");
      c = getchar();
	  getchar();
      c = toupper(c);

      switch (c) {
      case 'R':
         for (int i = 1; i < 101; i++) {
            sprintf(fname, "./txt/doc%03d.txt", i); // 파일 이름을 형식에 맞게 생성하여 fname에 저장
            strncpy(document_name[dcount], fname, NAME_LENGTH);
            bulid_hash_table(fname, dcount);
            dcount++;
         }
         break;
      case 'S':
         printf("\n Search Word: ");
         scanf("%s", key);
		 getchar();
         search_word(key, dcount);
         break;
      case 'I':
         printf("\n");
         print_idx_result(dcount);
         break;
      case 'Q':
         printf("\n");
         exit(1);
      default:
         break;
      }
   }
}


int transform(char* key) {
   int n = 0;
   while (key == NULL)
      n += *key++;
   return n;
}//folding 방식으로 key의 각 character 값을 더해 숫자로 변환하는 함수


void search_word(char* key, int d)
{
    int i;
    int max_count = 0; // 최대 출현 횟수 변수

    // 검색 결과를 저장할 배열
    HeapNode search_results[MAX_NUM_DOC];
    int num_results = 0; // 검색 결과 개수

    // 검색 결과를 search_results 배열에 저장
    for (i = 0; i < d; i++) {
        treePtr p = hash_table[i][hash(key)];
        treePtr result = search_bst(key, p);

        if (result != NULL) { // 검색어가 발견됬다면
            search_results[num_results].count = i;
            strcpy(search_results[num_results].document_name, document_name[i]);
            strcpy(search_results[num_results].data, result->data);
            search_results[num_results].count = result->count;

            if (result->count > max_count) {
                max_count = result->count;
            }

            num_results++;
        }
    }

	printf("Total Documents : %d\n", num_results);

    // 최대 출현 횟수가 높은 순서대로 결과를 출력
    for (int count = max_count; count > 0; count--) {
        for (int j = 0; j < num_results; j++) {
            if (search_results[j].count == count) {
				printf("\n");
                printf("<%s> %s : %d\n", search_results[j].document_name, search_results[j].data, search_results[j].count);
                print_doc(search_results[j].document_name, &search_results[j].data);
            }
        }
    }

    printf("\nTotal number of comparison = %d\n", num_search);
    num_search = 0;
}


void make_bst(char* key, treePtr p, treePtr temp)
{
   indexed_word++;
   treePtr a;
   a = p;
   int h = 0;
   while (a) {
      num_comparison++;//색인 비교연산 수 증가
      if (strcmp(key, a->data) == 0)
      {
         a->count++;
         return;
      }//만약 a의 데이터와 키가 같다면, a의 출현횟수를 증가시킨다.
      p = a;
      if (strcmp(key, a->data) < 0)
         a = a->left;
      else
         a = a->right;//a의 데이터와 키가 다르다면, 일치하는 데이터를 찾아 내려간다.
   }
   if (p != NULL)
   {
      if (strcmp(key, p->data) < 0)
         p->left = temp;
      else
         p->right = temp;
   }//일치하는 데이터가 없다면 노드를 삽입.
}//해시테이블에 이진탐색트리를 연결하는 함수


treePtr search_bst(char* key, treePtr p)
{
   treePtr ptr = p;
   while (ptr != NULL) {
      num_search++;//키와 노드의 데이터값 비교가 일어나므로 검색 비교연산의 횟수를 증가시킨다.
      if (strcmp(key, ptr->data) == 0)
         return ptr;//키와 노드의 데이터 값이 같다면 이 노드를 반환.
      if (strcmp(key, ptr->data) < 0)
         ptr = ptr->left;
      else
         ptr = ptr->right;//다르다면 원하는 노드를 찾아 내려감.
   }
   return NULL;
}//이진탐색트리를 검색하여 원하는 노드를 반환하는 함수. 찾는 데이터가 없다면 NULL을 반환.


void bulid_hash_table(char* fname, int d)
{
   char line[1200];
   char* token;
   FILE* fp;
   fp = fopen(fname, "r");//파일 입출력 스트림을 읽기모드로 연다.
   if (fp == NULL)
   {
      printf("There is no file.\n");
      return;
   }//파일이 존재하지 않는다면 파일을 읽어올 수 없다.
   else {
      while (fgets(line, 256, fp) != NULL)//파일을 끝날때까지 한줄씩 읽는다.
      {
         token = strtok(line, delimiter);//토큰을 분리해낸다.
         while (token != NULL)
         {
            insert_hash_table(token, d);
            token = strtok(NULL, delimiter);
         }//이 토큰을 해시테이블에 삽입하고, 토큰을 또 나누는 것을 파일이 끝날때까지 반복한다.
      }
      printf("<%s> File indexig is completed.\n", fname);//파일 색인이 끝났음을 표시.
   }
   fclose(fp);//파일 입출력 스트림을 닫는다.
}//파일을 색인하여 해시테이블을 만드는 함수이다.


void insert_hash_table(char* key, int d)
{
   treePtr temp = (treePtr)malloc(sizeof(node));
   strcpy(temp->data, key);
   temp->count = 1;
   temp->left = temp->right = NULL;//트리의 노드를 동적 할당하고, 내용을 초기화한다.
   treePtr q = hash_table[d][hash(key)];
   if (q == NULL)
   {
      num_comparison++;//색인시에 일어나는 비교 연산의 수를 증가시킨다.
      indexed_word++;
      hash_table[d][hash(key)] = temp;
   }
   else
      make_bst(key, q, temp);
   //만약 hash_table[d][hash(key)]에 노드가 없다면 그곳에 노드 삽입. 이미 노드 존재한다면 이진탐색트리를 만든다.
}//해시 테이블에 자료를 삽입하는 함수


int hash(char* key) {
   return transform(key) % 30;
}//folding값을 30으로 나눈 나머지를 해시함수 값으로 이용


void print_idx_result(int i)
{
   printf("\nTotal number of documents = %d \n", i);
   printf("Total number of indexed words = %d \n", indexed_word);
   printf("Total number of comparison = %d\n", num_comparison);
}//색인 결과를 프린트한다.


void print_result(treePtr p, int i)
{

   if (p) {
      printf("\n<%s> %s : %d", document_name[i], p->data, p->count);
   }
}//검색 후 결과를 프린트함.


void print_doc(char* fname, treePtr p)
{
   char line[1000];
   char printLine[1000];
   char* token, * token2;
   FILE* fp;
   fp = fopen(fname, "r");//파일 입출력 스트림을 연다
   if (fp == NULL)
   {
      printf("There is no file.\n");
      return;
   }//파일이 존재하지 않는다면, 출력을 수행할 수 없음을 표시한다.
   while (fgets(line, 64, fp) != NULL)//파일에서 32개의 문자씩 읽어온다
   {
      strcpy(printLine, line);
      token = strtok(line, delimiter);
      while (token != NULL)
      {
         if (strcmp(token, p->data) == 0) { //만약 token이 찾는 단어와 일치한다면
            printf("\n * %s", printLine);
         }//그 token이 속하는 문장을 출력한다.

         token = strtok(NULL, delimiter);
      }
   }
   printf("\n");
   fclose(fp);//파일 입출력 스트림을 닫는다.
}//검색 결과가 있는 문서의 위치를 프린트하는 함수