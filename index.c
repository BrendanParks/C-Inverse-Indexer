#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>

#include <fts.h>
#include "index.h"
#include "tokenizeralphanum.c"
/*Just a compare funct right now*/
#include "hash_functs.h"
#include "sl/sorted-list.h"
#include "sl/sorted-list.c"
#include "ext/uthash.h"


/*Traverse and index one file*/
int traverseFile() {
	return 0;
}

void destroyWordList(WordHashNode wordList) {
	WordHashNode w;
	
	for(w = wordList; w != NULL; w=w->hh.next) {
		/*Free word from hash*/
		free (w->word);
		
		/*Free filenames from sorted list*/
		SortedListIteratorPtr iter;
		iter = SLCreateIterator(w->files);
		void *obj = NULL;
		while ((obj = SLNextItem(iter))) {
			FileWithCount objFile;
			objFile = obj;
			
			free(objFile->filename);
		}
		SLDestroyIterator(iter);
		
		/*Free sorted list*/
		SLDestroy(w->files);
		
		/*Free word list node*/
		HASH_DEL(wordList, w);
		free( w );
		
	}


}

void writeWordListToFile(char *filename, WordHashNode wordList) {

	return;

}

void add_word(WordHashNode wordList, char *filename, char *wordToAdd, char *tokenString) {
	WordHashNode iterw = NULL;
	WordHashNode w = NULL;
	FileWithCount file;
	file = (FileWithCount) malloc( sizeof(FileWithCount) );
	file->filename = (char *) malloc( BUFSIZ+1 );
	strncat(file->filename,filename,BUFSIZ);
	file->count = 1;
	
	/*Find if word exists*/
    for(iterw = wordList; iterw != NULL; iterw = iterw->hh.next) {
		printf("WORD WORD WROD: %s\n",iterw->word);
		if (strcmp(iterw->word, wordToAdd) == 0) { /*Word exists*/
			w = iterw;
		}
    }	
	
	HASH_FIND_STR( wordList, "include", w);
	
	if (w != NULL) { 				/*Word already exists*/
		printf("WORD EXISTS:  %s\n", wordToAdd);
		SortedListIteratorPtr iter;
		iter = SLCreateIterator(w->files);
		void *obj = NULL;
		while ((obj = SLNextItem(iter))) {
			FileWithCount objFile;
			objFile = obj;		

			if (compareFilesWithCount(objFile, file) == 0) {
			/*Filename present,
				just increase count and break*/
				objFile->count++;
				break;
			}
		}
		SLDestroyIterator(iter);
		
		if(obj == NULL) {
		/*File not found, insert it into list*/
			SLInsert(w->files, file);	
			
		} else {
			fprintf(stderr, "ERROR: Add Word\n");
			free (file->filename);
			free (file);
		}

		return;
	} else {						/*Word doesn't exist yet*/

		w = (WordHashNode) malloc( sizeof(WordHashNode) );
		w->word = (char *) malloc( BUFSIZ+1 );
		w->word[0] = '\0';
		strncpy(w->word, wordToAdd, BUFSIZ);
		
		w->files = SLCreate(compareFilesWithCount);
		/* printf("The word of the day is %s\n", w->word); */
		
		SLInsert(w->files, file);

		HASH_ADD_KEYPTR(hh, wordList, w->word, strlen(w->word), w); /* word: name of key field */

		return;
	}
	
}

int addStringToHash(char *filename, char *string, WordHashNode wordList) {

	if (string == NULL || filename == NULL) return 1;
	
	TokenizerAlphaNumT* tokenizer;
	tokenizer = TKANCreate(string);
	if(tokenizer == NULL) {
		printf("Error: unable to create tokenizer\n");
		return 1;
	}
	
	char* token = NULL;
	/* while((token = TKANGetNextToken(tokenizer)) != NULL) {  */
	
	while((token = TKANGetNextToken(tokenizer)) != NULL) {
		printf("Before add_word: %s\n",tokenizer->copied_string);
		/*Add to hash here*/
		add_word(wordList, filename, token, tokenizer->copied_string);
		printf("After add_word: %s\n",tokenizer->copied_string);	
		
		
		/* printf("%s", token); */
		free(token);
	}
	printf("\nDestroying Tokenizer...: %s\n",tokenizer->copied_string);
	if (tokenizer != NULL) TKANDestroy(tokenizer);
	
	return 0;
}

/*Begin steps to open and index single file*/
int openAndIndexFile(char *filename, WordHashNode wordList) {
	FILE *fp;
	char buf[BUFSIZ+1];

	fp = fopen(filename,"r");
	if(fp == NULL) {
	printf("OH SHIT OH SHIT\n");
	   fprintf(stderr, "ERROR: opening file failed.\n");
	   return 1;
	}
	printf("OPENED FILEe: %s\n", filename);
	
	
	
	while( fgets(buf, BUFSIZ+1, fp) ) {
		addStringToHash(filename, buf, wordList);
	}
	
	fclose(fp);
	
	/*Loop through file*/
	
	
	/*Only supports ASCII, could ask for encoding in future,
	 or maybe check encoding of file in future?*/
	return 0;
}

int openAndIndexDirectory(char *dir, WordHashNode wordList) {
	char *directory;
	directory = (char *) malloc (BUFSIZ);
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
	
	char *fullPath;
	
	
	while ((parent = fts_read(ftsp)) != NULL ) {
		
		switch(parent->fts_info) {
			case FTS_F:
				
				fullPath = (char *) malloc( BUFSIZ );
				fullPath[0] = '\0';
				strncat(fullPath, parent->fts_path, BUFSIZ - 1);
				/* strncat(fullPath, parent->fts_name, BUFSIZ - 1); */
				printf("FILE HERE:%s\n",fullPath);
				openAndIndexFile(fullPath, wordList);
				break;
			default:
				printf("HMM...\n");
				break;
		}
	}
	
	fts_close(ftsp);
	
	if (fullPath) free (fullPath);
	return 0;

}

int main(int argc, const char **argv) {

	/*Check for args*/
	if (argc != 3) {
		fprintf(stderr, "USAGE: index <inverted-index file name> "
						"<directory or file name>\n");
		return 1;
	}
	/*HASH TABLE DECLARATION*/
	WordHashNode wordList = NULL;
		
	/*Check if arg2 is directory or file*/
	struct stat s;
	if( stat(argv[2],&s) == 0 ) {
		if( s.st_mode & S_IFDIR ) {
			/*Directory*/
			char *dir;
			dir = (char *) malloc ( BUFSIZ );
			dir = strncpy(dir,argv[2],BUFSIZ - 1);
			openAndIndexDirectory(dir, wordList);
		}
		else if( s.st_mode & S_IFREG ) {
			/*File*/
			char *file;
			file = (char *) malloc ( BUFSIZ );
			file = strncpy(file,argv[2],BUFSIZ - 1);			
			openAndIndexFile(file, wordList);
		}
		else {
			/*Don't know what it is*/
			fprintf(stderr,"STAT: Unknown data type\n");
			return EXIT_FAILURE;
		}
	}
	else {
		fprintf(stderr,"STAT: Unknown fatal error (Check file/dir name)\n");
		return EXIT_FAILURE;
	}
		
	FILE *textFile;
	


}

