#define NAME_LENGTH 20
#define WORD_LENGTH 20
#define MAX_NUM_DOC 10
#define delimiter " \"\n\t`'…?!.,:()"

typedef struct treeNode *treePtr;
typedef struct treeNode {
	char data[WORD_LENGTH];
	int  count;
	treePtr left;
	treePtr right;
}node;
treePtr hash_table[MAX_NUM_DOC][30];
int num_comparison = 0;
int num_search = 0;
char document_name[MAX_NUM_DOC][NAME_LENGTH];
int indexed_word = 0;

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