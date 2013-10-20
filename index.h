#include "ext/uthash.h"
#include "sl/sorted-list.h"
struct WordHashNode {
	char *word;			/*KEY: Word*/
	SortedListPtr files;		/*VALUE: List of file locations*/
	UT_hash_handle hh;	/*Makes struct hashable*/


}; typedef struct WordHashNode *WordHashNode;

struct WordNode {
	char *word;			/*KEY: Word*/
	SortedListPtr files;		/*VALUE: List of file locations*/
}; typedef struct WordNode *WordNode;

struct FileWithCount {
	char *filename;
	int count;
}; typedef struct FileWithCount *FileWithCount;



void addWord();
void deleteWord();
WordHashNode findWord();
