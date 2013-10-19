#include "ext/uthash.h"
#include "sl/sorted-list.h"
struct WordHashNode {
	char *word;			/*KEY: Word*/
	SortedListPtr files;		/*VALUE: List of file locations*/
	UT_hash_handle hh;	/*Makes struct hashable*/


}; typedef struct WordHashNode *WordHashNode;

void addWord();
void deleteWord();
WordHashNode findWord();
