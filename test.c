#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <err.h>

#include <fts.h>
#include "index.h"





/*Traverse and index one file*/
int traverseFile() {
	return NULL;
}

/*Begin steps to open and index single file*/
int openAndIndexFile(char *filename) {
	return NULL;
}

int openAndIndexDirectory(char *dir) {
	char *dir;
	dir = (char *) malloc (sizeof(BUFSIZ));
	
	
	FTS *ftsp;
	FTSENT *parent, *chp;
	int fts_options = FTS_COMFOLLOW | FTS_LOGICAL | FTS_NOCHDIR;
	int rval = 0;
	
	if((ftsp = fts_open(directory, fts_options, NULL)) == NULL) {
		warn("fts_open");
		return -1;
	}
	
	/*If no children in directory, return */
	chp = fts_children(ftsp, 0);
	if (chp == NULL) return 0;
	
	while ((parent = fts_read(ftsp)) != NULL ) {
		switch(parent->fts_info) {
			case FTS_D:

                printf("DIR %s%s\n", parent->fts_path, parent->fts_name);
				break;
			case FTS_F;
                printf("FILE %s%s\n", parent->fts_path, parent->fts_name);
				break;
			default:
				break;
		}
	}
	
	fts_close(ftsp);
	
	return 0;
	
	


}

int main(int argc, const char **argv) {

	/*Check for args*/
	if (argc != 3) {
		fprintf(stderr, "USAGE: index <inverted-index file name> <directory or file name>

	/*Init words hash table*/
	WordHashNode wordList = NULL;
		
	/*Check if arg2 is directory or file*/
	struct stat s;
	if( stat(argv[2],&s) == 0 ) {
		if( s.st_mode & S_IFDIR ) {
			/*Directory*/
			openAndIndexDirectory(argv[2]);
		}
		else if( s.st_mode & S_IFREG ) {
			/*File*/
			openAndIndexFile(argv[2]);
		}
		else {
			/*Don't know what it is*/
			fprintf(stderr,"STAT: Unknown data type\n");
			return EXIT_FAILURE;
		}
	}
	else {
		fprintf(stderr,"STAT: Unknown fatal error\n");
		return EXIT_FAILURE;
	}
		
	FILE *textFile;
	


}

