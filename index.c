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


/*Word list DECLARATION*/
SortedListPtr wordList;

#define BUFSIZ 1024

/*Traverse and index one file*/
int traverseFile() {
	return 0;
}

void destroyWordList() {
	
	
	SortedListIteratorPtr wordIterator = NULL;
	wordIterator = SLCreateIterator(wordList);
	WordNode curWord;
	
	SortedListIteratorPtr fileIterator = NULL;
	FileWithCount curFile;
	
	void *obj = NULL;
	void *objInWord = NULL;
	
	while ((obj = SLNextItem(wordIterator))) {
		curWord = (WordNode) obj;
		
		free (curWord->word);
		
		fileIterator = SLCreateIterator(curWord->files);
		
		while ((objInWord = SLNextItem(fileIterator))) {		
			curFile = (FileWithCount) objInWord;
			
			free(curFile->filename);
			
		
		}
		SLDestroyIterator(fileIterator);
		SLDestroy(curWord->files);
	}
	
	SLDestroyIterator(wordIterator);
	SLDestroy(wordList);

}

void writeWordListToFile(char *filename) {
	/*Traverse wordlist, grab each word*/

	FILE *f = fopen(filename, "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}

	/* print some text */
	const char *text = "Write this to the file";
	fprintf(f, "Some text: %s\n", text);



	
	SortedListIteratorPtr wordIterator = NULL;
	wordIterator = SLCreateIterator(wordList);
	WordNode curWord;
	
	SortedListIteratorPtr fileIterator = NULL;
	FileWithCount curFile;
	
	void *obj = NULL;
	void *objInWord = NULL;
	
	while ((obj = SLNextItem(wordIterator))) {
		curWord = (WordNode) obj;
		
		fprintf(f,"<list> %s\n",curWord->word);
		
		fileIterator = SLCreateIterator(curWord->files);
		
		while ((objInWord = SLNextItem(fileIterator))) {		
			curFile = (FileWithCount) objInWord;
			
			fprintf(f,"%s %d ",curFile->filename,curFile->count);
			
		
		}
		SLDestroyIterator(fileIterator);
		fprintf(f,"\n</list>\n",curWord->word);
	}
	
	SLDestroyIterator(wordIterator);
	
	fclose(f);
	return;

}
 
void add_word(char *filename, char *wordToAdd, char *tokenString) {

	FileWithCount file;
	file = (FileWithCount) malloc( sizeof(FileWithCount) );
	file->filename = (char *) malloc( BUFSIZ+1 );
	snprintf(file->filename, BUFSIZ, "%s", filename);
		
	file->count = 1;
	printf("Filename after file %s and should be %s\n",file->filename,filename);

	/*Find if word exists*/
	WordNode wordToFind = (WordNode) malloc(sizeof(WordNode));
	wordToFind->word = (char *) malloc (BUFSIZ+1);
	snprintf(wordToFind->word, BUFSIZ, "%s", wordToAdd);
	
	WordNode isWordThere;
	isWordThere = (WordNode) SLFind(wordList, wordToFind);
	
	
	if (isWordThere != NULL) { 				/*Word already exists*/
		printf("WORD EXISTS:  %s\n", wordToAdd);
		
		SortedListIteratorPtr iter;
		iter = SLCreateIterator(isWordThere->files);
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
			printf("Filename before slinsert %s\n",file->filename);
			SLInsert(isWordThere->files, file);
			printf("Inserted file %s into word %s\n",file->filename,isWordThere->word);
			printf("Filename actually %s\n",filename);
			free (wordToFind->word);
			free (wordToFind);
			
		} else {
			fprintf(stderr, "ERROR: Add Word\n");
			free (file->filename);
			free (file);
			free (wordToFind->word);
			free (wordToFind);			
		}

		return;
	} else {						/*Word doesn't exist yet*/

		wordToFind->files = SLCreate(compareFilesWithCount);
		/* printf("The word of the day is %s\n", w->word); */
		
		/*Insert file path into word*/
		SLInsert(wordToFind->files, file);
		/*Insert word into word list*/
		SLInsert(wordList, wordToFind);

		return;
	}
	
}

int addStringToHash(char *filename, char *string) {

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
		printf("BEFORE LOWER CASE %s\n",token);
		  int i;
		  for (i = 0; token[i]; i++)
		  token[i] = tolower(token[ i ]);		
		
		printf("AFTER LOWER CASE %s\n",token);
		/*Add to hash here*/
		
		add_word(filename, token, tokenizer->copied_string);

		/* printf("%s", token); */
		free(token);
	}
	
	if (tokenizer != NULL) TKANDestroy(tokenizer);
	
	return 0;
}

/*Begin steps to open and index single file*/
int openAndIndexFile(char *filename) {
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
		addStringToHash(filename, buf);
	}
	
	fclose(fp);
	
	/*Loop through file*/
	
	
	/*Only supports ASCII, could ask for encoding in future,
	 or maybe check encoding of file in future?*/
	return 0;
}

int openAndIndexDirectory(char *dir) {
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
				
				fullPath = (char *) malloc( BUFSIZ+1 );

				snprintf(fullPath, BUFSIZ, "%s", parent->fts_path);
				/* strncat(fullPath, parent->fts_name, BUFSIZ - 1); */
				printf("FILE HERE:%s\n",fullPath);
				openAndIndexFile(fullPath);
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
	if (argv[2][0] == ')') {
			/*Don't allow from root directory*/
			fprintf(stderr,"ERROR: Indexing from root is disallowed!\n");
			return EXIT_FAILURE;
	}
	wordList = SLCreate(compareWords);
	/*Check if arg2 is directory or file*/
	struct stat s;
	if( stat(argv[2],&s) == 0 ) {
		if( s.st_mode & S_IFDIR ) {
			/*Directory*/
			char *dir;
			dir = (char *) malloc ( BUFSIZ );
			snprintf(dir, BUFSIZ-1, "%s", argv[2]);
			
			openAndIndexDirectory(dir);
			free (dir);
		}
		else if( s.st_mode & S_IFREG ) {
			/*File*/
			char *file;
			file = (char *) malloc ( BUFSIZ );
			snprintf(file, BUFSIZ-1, "%s", argv[2]);
						
			openAndIndexFile(file);
			free (file);
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
	/*Write inverted index to specified file*/
	writeWordListToFile(argv[1]);
	/*Destroy word list*/
	destroyWordList();
	


}

