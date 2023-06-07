#define NAME_LENGTH 100
#define WORD_LENGTH 100
#define MAX_NUM_DOC 100
#define delimiter " \"\n\t`'…?!.,:()"
#define MAX_RESULTS 100

typedef struct treeNode *treePtr;
typedef struct treeNode {
	char data[WORD_LENGTH];
	int  count;
	treePtr left;
	treePtr right;
}node;

typedef struct {
    int doc_index;
    char document_name[NAME_LENGTH];
    char data[WORD_LENGTH];
    int count;
} HeapNode;

treePtr hash_table[MAX_NUM_DOC][5000];
int num_comparison = 0;
int num_search = 0;
char document_name[MAX_NUM_DOC][NAME_LENGTH];
int indexed_word = 0;

void readFilesInFolder(const char *folderPath, int *dcount);
int transform(char *key);
void search_word(char *key, int d);
void make_bst(char *key, treePtr p, treePtr temp);
treePtr search_bst(char*key, treePtr p);
void bulid_hash_table(char*fname, int d);
void insert_hash_table(char*key, int d);
int hash(char*key);
void print_idx_result(int i);
void print_result(treePtr p, int i);
void print_doc(char *fname, treePtr p);

void heapify(HeapNode arr[], int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l].count > arr[largest].count)
        largest = l;

    if (r < n && arr[r].count > arr[largest].count)
        largest = r;

    if (largest != i) {
        HeapNode temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;

        heapify(arr, n, largest);
    }
}

void heap_sort(HeapNode arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        HeapNode temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        heapify(arr, i, 0);
    }
}