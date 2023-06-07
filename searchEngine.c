#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "searchEngine.h"


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

// folding 방식
int transform(char *key)
{ // A-Z(0-25) a-z(26-51)
    int num = 0;
    for (int i = 0; i < strlen(key); i++)
    {
        if (key[i] >= 'A' && key[i] <= 'Z')
            num = num + key[i] - 'A'; // A의 값이 0
        else if (key[i] >= 'a' && key[i] <= 'z')
            num = num + key[i] - 'a' + 26; // a의 값이 26
    }
    return num;
}

int hash(char* key) {
   return transform(key) % 5000;
} // folding값을 30으로 나눈 나머지를 해시함수 값으로 이용


void search_word(char* key, int d) {
    int i;
    int max_count = 0;
    SearchResult search_results[MAX_NUM_DOC];
    int num_results = 0;

    for (i = 0; i < d; i++) {
        treePtr p = hash_table[i][hash(key)];
        treePtr result = search_bst(key, p);

        if (result != NULL) {
            search_results[num_results].count = result->count;
            strcpy(search_results[num_results].document_name, document_name[i]);
            strcpy(search_results[num_results].data, result->data);

            if (result->count > max_count) {
                max_count = result->count;
            }

            num_results++;
        }
    }

    printf("Total Documents: %d\n", num_results);

    heapSort(search_results, num_results);

    for (int j = num_results-1; j >= 0; j--) {
        printf("\n");
        printf("<%s> %s : %d\n", search_results[j].document_name, search_results[j].data, search_results[j].count);
        print_doc(search_results[j].document_name, &search_results[j].data);
    }

    printf("\nTotal number of comparison = %d\n", num_comparison_at_search);
    num_comparison_at_search = 0;
}

treePtr search_bst(char* key, treePtr p)
{
   treePtr ptr = p;
   while (ptr != NULL) {
      num_comparison_at_search++;//키와 노드의 데이터값 비교가 일어나므로 검색 비교연산의 횟수를 증가시킨다.
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
   char line[256];
   char* token;
   FILE* fp;
   fp = fopen(fname, "r"); // 해당 파일을 읽는다
   if (fp == NULL) // 아무것도 안들어있으면
   {
      printf("There is no file.\n");
      return;
   }
   else { // 내용이 있다면
      while (fgets(line, 256, fp) != NULL) // 파일을 끝날때까지 한 줄씩 읽는다.
      {
         token = strtok(line, delimiter); // 한 줄의 모든 토큰을 분리해낸다.
         while (token != NULL) // 토큰이 존재하는동안
         {
            insert_hash_table(token, d); // token을 hash table에 insert
            token = strtok(NULL, delimiter); // 다음 token으로 update
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
   temp->left = temp->right = NULL; // 트리의 노드를 동적 할당하고, 내용을 초기화한다.

   treePtr q = hash_table[d][hash(key)];
   if (q == NULL) // hash_table에 처음 삽입되는 단어라면
   {
      total_comparison_at_indexed++; // 색인시에 일어나는 비교 연산의 수를 증가시킨다.
      indexed_word++; // 색인 된 단어의 수를 하나 증가시킨다
      hash_table[d][hash(key)] = temp; // temp를 hash table에 삽입한다
   }
   else{ // 이미 안에 해당 단어가 있다면
      make_bst(key, q, temp); // bst를 형성한다
   }
   //만약 hash_table[d][hash(key)]에 노드가 없다면 그곳에 노드 삽입. 이미 노드 존재한다면 이진탐색트리를 만든다.
}//해시 테이블에 자료를 삽입하는 함수

void make_bst(char* key, treePtr p, treePtr temp)
{
   indexed_word++;
   treePtr a;
   a = p;

   while (a) {
      total_comparison_at_indexed++; // 색인 비교연산 수 증가
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


void print_idx_result(int i)
{
   printf("\nTotal number of documents = %d \n", i); // 파일 갯수
   printf("Total number of indexed words = %d \n", indexed_word); // 색인된 단어 수
   printf("Total number of comparison = %d\n", total_comparison_at_indexed); // 색인 시 비교 횟수
}//색인 결과를 프린트한다.


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

void swap(SearchResult* a, SearchResult* b) {
    SearchResult temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(SearchResult arr[], int n, int i) { // heap 유지
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].count > arr[largest].count)
        largest = left;

    if (right < n && arr[right].count > arr[largest].count)
        largest = right;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest); // 재귀적 호출
    }
}

void heapSort(SearchResult arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i >= 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}