#define NAME_LENGTH 100
#define WORD_LENGTH 100
#define MAX_NUM_DOC 100
#define delimiter " \"\n\t`'…?!.,:()"
#define MAX_RESULTS 100

typedef struct treeNode *treePtr; // treeNode형 포인터
typedef struct treeNode { // treenode 구조체
	char data[WORD_LENGTH]; // 어떤 단어에 대한 구조체?
	int  count; // 빈도 수, 얼마나 많이 나왔나
	treePtr left; 
	treePtr right;
}node;

typedef struct {
    char document_name[NAME_LENGTH];
    char data[WORD_LENGTH];
    int count;
} SearchResult;


treePtr hash_table[MAX_NUM_DOC][5000]; // treePtr형 hash_table
int total_comparison_at_indexed = 0; // 총 비교 횟수
int num_comparison_at_search = 0; // 특정 단어 검색 시 비교 횟수
char document_name[MAX_NUM_DOC][NAME_LENGTH]; // 문서 이름
int indexed_word = 0; // 인식된 단어 개수

int hash(char*key); // hash 함수
int transform(char *key); // hash fording 방식

void search_word(char *key, int d);
void make_bst(char *key, treePtr p, treePtr temp);
treePtr search_bst(char*key, treePtr p);
void bulid_hash_table(char*fname, int d);
void insert_hash_table(char*key, int d); 
void print_idx_result(int i); // 파일 수, 색인 된 단어 횟수, 색인 시 비교횟수 출력
void print_doc(char *fname, treePtr p); // 특정 단어가 포함된 문장 출력

void heapSort(SearchResult arr[], int n);
void heapify(SearchResult arr[], int n, int i);
void swap(SearchResult* a, SearchResult* b);