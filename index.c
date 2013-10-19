#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
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
	fclose(fp);

	return 0;
}

int openAndIndexDirectory(char *dir) {
	char *directory;
	directory = (char *) malloc (sizeof(BUFSIZ));
	directory[0] = '\0';
	strcat(directory, dir);
	
	/*We need this to conform to how fts works*/
	/*fts needs an array of char arrays for some reason*/
	char *directoryArg[2];
	directoryArg[0] = directory;
	directoryArg[1] = NULL;
	
	FTS *ftsp;
	FTSENT *parent, *chp;
	int fts_options = FTS_COMFOLLOW | FTS_LOGICAL | FTS_NOCHDIR;

	printf("DIR ARG IS %s\n",directoryArg);
	if((ftsp = fts_open(directoryArg, fts_options, NULL)) == NULL) {
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
				strncat(fullPath, parent->fts_path, BUFSIZ - 1);
				/* strncat(fullPath, parent->fts_name, BUFSIZ - 1); */
				printf("DIRECTPORY...%s\n",fullPath);
				openAndIndexDirectory(fullPath);
				free (fullPath);
				break;
			case FTS_F:
				
				fullPath = (char *) malloc( sizeof(BUFSIZ) );
				fullPath[0] = '\0';
				strncat(fullPath, parent->fts_path, BUFSIZ - 1);
				/* strncat(fullPath, parent->fts_name, BUFSIZ - 1); */
				printf("FIELLLLL.....%s\n",fullPath);
				openAndIndexFile(fullPath);
				free (fullPath);
				break;
			default:
				printf("HMM...\n");
				break;
		}
	}
	
	fts_close(ftsp);
	
	return 0;
	
	


}

int main(int argc, const char **argv) {

	/*Check for args*/
	if (argc != 3) {
		fprintf(stderr, "USAGE: index <inverted-index file name> "
						"<directory or file name>\n");
		return 1;
	}
	/*Init words hash table*/
	WordHashNode wordList = NULL;
		
	/*Check if arg2 is directory or file*/
	struct stat s;
	if( stat(argv[2],&s) == 0 ) {
		if( s.st_mode & S_IFDIR ) {
			/*Directory*/
			char *dir;
			dir = (char *) malloc ( sizeof(BUFSIZ) );
			dir = strncpy(dir,argv[2],BUFSIZ - 1);
			openAndIndexDirectory(dir);
		}
		else if( s.st_mode & S_IFREG ) {
			/*File*/
			char *file;
			file = (char *) malloc ( sizeof(BUFSIZ) );
			file = strncpy(file,argv[2],BUFSIZ - 1);			
			openAndIndexFile(file);
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

