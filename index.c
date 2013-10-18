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
	return 0;
}

/*Begin steps to open and index single file*/
int openAndIndexFile(char *filename) {
	FILE *fp;
	fp = fopen(filename,"r");
	

	return 0;
}

int openAndIndexDirectory(char *dir) {
	char *directory;
	directory = (char *) malloc (sizeof(BUFSIZ));
	directory[0] = '\0';
	strcat(directory, dir);
	
	FTS *ftsp;
	FTSENT *parent, *chp;
	int fts_options = FTS_COMFOLLOW | FTS_LOGICAL | FTS_NOCHDIR;

	
	if((ftsp = fts_open(directory, fts_options, NULL)) == NULL) {
		warn("fts_open");
		return -1;
	}
	
	/*If no children in directory, return */
	chp = fts_children(ftsp, 0);
	if (chp == NULL) return 0;
	
	while ((parent = fts_read(ftsp)) != NULL ) {
		char *fullPath;
		switch(parent->fts_info) {
			case FTS_D:
			
			/*  child = child->fts_link;
                printf("%s%s\n", child->fts_path, child->fts_name); */
				
				fullPath = (char *) malloc( sizeof(BUFSIZ) );
				fullPath[0] = '\0';
				strcat(fullPath, parent->fts_path);
				strcat(fullPath, parent->fts_name);
				openAndIndexDirectory(fullPath);
				free (fullPath);
				break;
			case FTS_F:

				fullPath = (char *) malloc( sizeof(BUFSIZ) );
				fullPath[0] = '\0';
				strcat(fullPath, parent->fts_path);
				strcat(fullPath, parent->fts_name);
				openAndIndexFile(fullPath);
				free (fullPath);
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
		fprintf(stderr, "USAGE: index <inverted-index file name> <directory or file name>");

	/*Init words hash table*/
	WordHashNode wordList = NULL;
		
	/*Check if arg2 is directory or file*/
	struct stat *s;
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

