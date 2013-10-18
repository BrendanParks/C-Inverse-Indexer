#include "ext/uthash.h"

struct WordHashNode {
	char *word;			/*KEY: Word*/
	SortedListPtr files;		/*VALUE: List of file locations*/
	UT_hash_handle hh;	/*Makes struct hashable*/


}; typedef struct WordHashNode *WordHashNode;

void addWord();
void deleteWord();
WordHashNode findWord();
