/*
 * sorted-list.c
 */

#include	<string.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	"sorted-list.h"

int compareInts(void *p1, void *p2)
{
	int i1 = *(int*)p1;
	int i2 = *(int*)p2;

	return i1 - i2;
}

int compareDoubles(void *p1, void *p2)
{
	double d1 = *(double*)p1;
	double d2 = *(double*)p2;

	return (d1 < d2) ? -1 : ((d1 > d2) ? 1 : 0);
}

int compareStrings(void *p1, void *p2)
{
	char *s1 = p1;
	char *s2 = p2;

	return strcmp(s1, s2);
}

/*Load sorted list with ints up to 100*/
int loadIntList(SortedListPtr intList) {
	if (intList == NULL) return 0;
	int i = 0;
	int *number = NULL;
	for (;i < 101; i++) {
		number = malloc( sizeof(int));
		*number = i;
		SLInsert(intList, number);
	}
	
	return 1;
}

/*Load sorted list with ints up to 10*/
int loadIntListSmallThenRemove(SortedListPtr intList) {
	if (intList == NULL) return 0;
	int i = 0;
	int *number = NULL;
	for (;i < 11; i++) {
		number = malloc( sizeof(int));
		*number = i;
		SLInsert(intList, number);
	}
	
	
	for (i = 0;i < 11; i=i+2) {
		number = malloc( sizeof(int));
		*number = i;
		SLRemove(intList, number);
		free (number);
	}	
	
	return 1;
}

/*Load sorted list with doubles 0.0-1.09*/
int loadDoubleList(SortedListPtr doubleList) {
	if (doubleList == NULL) return 0;
	double i = 0.0f;
	double *number = NULL;
	for (;i < 1.1f; i=i+0.01f ) {
		number = malloc( sizeof(double));
		*number = i;
		SLInsert(doubleList, number);
	}
	
	return 1;
}

/*Load sorted list with 30 random 3-character a-z strings*/
int loadStringList(SortedListPtr stringList) {
	if (stringList == NULL) return 0;
	
	int i = 0;
	char *string;
	for (;i < 30; i++) {
		string = malloc( 4*sizeof(char));
		/*Generate 3 random letters for each string*/
		string[0] = (rand() % (122+1-97))+97;
		string[1] = (rand() % (122+1-97))+97;
		string[2] = (rand() % (122+1-97))+97;
		string[3] = '\0';
		
		SLInsert(stringList, string);
	}
	
	return 1;
}

void testIntList(SortedListPtr intList, int flag) {

	intList = SLCreate(compareInts);

	/*Insert 100 numbers into int list*/
	
	int i = 0;
	
	printf(">>STORING 0-100 IN LIST...\n");
	if (flag) {
		if (loadIntList(intList))
			printf("...OK!\n");
		else{
			printf("...FAIL! Returning...\n");
			return;
		}		
	
	} else {
		if (loadIntListSmallThenRemove(intList))
			printf("...OK!\n");
		else{
			printf("...FAIL! Returning...\n");
			return;
		}	
			
	}

	SortedListIteratorPtr iteratorInt = NULL;
	iteratorInt = SLCreateIterator(intList);
	
	void *obj = NULL;
	/*Check if iterators are working*/
	printf(">>>PRINTING STORED NUMBERS...\n");
	int *num;
	while ((obj = SLNextItem(iteratorInt))) {
		num = (int*) obj;
		printf("%d ", *num);
		i++;
	}
	if (flag) printf("\nSHOULD SEE: numbers in descending order from 100 to 0\n");
	else printf("\nSHOULD SEE: numbers in descending order from 10 to 0, even numbers removed\n");
	printf(">>!>>KILLING LIST AND ITERATOR! \n");
	SLDestroy(intList);
	SLDestroyIterator(iteratorInt);
	printf("...LIST AND ITERATOR KILLED SUCCESSFULLY!\n\n");
}

void testIteratorsUsingStrings(SortedListPtr stringList) {
	
	stringList = SLCreate(compareStrings);
	
	printf(">>STORING 30 3-LETTER RANDOM STRINGS IN LIST...\n");
	if (loadStringList(stringList))
		printf("...OK!\n");
	else{
		printf("...FAIL! Returning...\n");
		return;
	}
	
	SortedListIteratorPtr iteratorString = NULL;
	iteratorString = SLCreateIterator(stringList);
	
	int i = 0;
	void *obj = NULL;
	char *str;
	printf(">>PRINTING STRINGS IN LIST...\n");
	while ((obj = SLNextItem(iteratorString))) {
		str = (char*) obj;
		printf("%s ", str);
		i++;
		
		if (i == 15) break;
	}
	printf("\nSHOULD SEE: 15 3-character-long strings in reverse alphabetical"
				"order.\n");
	printf("\n>>!>>KILLING LIST AND ITERATOR! \n");
	SLDestroy(stringList);
	SLDestroyIterator(iteratorString);
	printf("...LIST AND ITERATOR KILLED SUCCESSFULLY!\n\n");

}

void testMultipleIteratorsUsingDoubles(SortedListPtr doubleList) {
	doubleList = SLCreate(compareDoubles);
	
	printf(">>STORING 0.01-1.0 DOUBLE VALUES IN LIST...\n");
	if (loadDoubleList(doubleList))
		printf("...OK!\n");
	else{
		printf("...FAIL! Returning...\n");
		return;
	}
	
	SortedListIteratorPtr iteratorDouble = NULL;
	SortedListIteratorPtr iteratorDoubleTwo = NULL;
	iteratorDouble = SLCreateIterator(doubleList);
	iteratorDoubleTwo = SLCreateIterator(doubleList);
	
	int i = 0;
	void *obj = NULL;
	void *objTwo = NULL;
	double *dbl;
	printf(">>PRINTING DOUBLES IN LIST...\n");
	while ( (obj = SLNextItem(iteratorDouble))
			&& (objTwo = SLNextItem(iteratorDoubleTwo))
			&& (obj == objTwo) ) {
		dbl = (double*) obj;
		printf("%f ", *dbl);
		dbl = (double*) objTwo;
		printf("%f\n", *dbl);
		i++;
	}
	printf("\nSHOULD SEE: Numbers 1.0 to 0.0, with duplicate.\n");
	printf("\nWHAT'S HAPPENING HERE: Two iterators are pointing to the\n"
			"same list and iterating at the same time. Numbers are only printed\n"
			"when what's returned from each iterator is the same thing, which\n"
			"should always be the case.\n");
	printf("\n>>!>>KILLING LIST AND ITERATORS! \n");
	SLDestroy(doubleList);
	SLDestroyIterator(iteratorDouble);
	SLDestroyIterator(iteratorDoubleTwo);
	printf("...LIST AND ITERATORS KILLED SUCCESSFULLY!\n\n");
}

int main (int argc, const char *argv[]){

	printf(">>>CREATING POINTERS TO LISTS...\n");
	SortedListPtr intList = NULL;
	SortedListPtr doubleList = NULL;
	SortedListPtr stringList = NULL;
	printf("...OK!\n");
	
	printf(">>>INSTANTIATING INT, DOUBLE, AND STRING LISTS...\n");
	
	 
	printf("...OK!\n\n");

	printf("=======================================\n");
	printf("INT TEST\n");
	printf("=======================================\n\n");
	testIntList(intList,1);
	
	printf("=======================================\n");
	printf("INT TEST TO 10, DELETE EVEN NUMBERS\n");
	printf("=======================================\n\n");
	testIntList(intList,0);
	
	printf("=======================================\n");
	printf("ITERATORS USING STRINGS TEST\n");
	printf("=======================================\n\n");
	testIteratorsUsingStrings(stringList);
	
	printf("=======================================\n");
	printf("MULTIPLE ITERATORS USING DOUBLES TEST\n");
	printf("=======================================\n\n");	
	testMultipleIteratorsUsingDoubles(doubleList);
	
	printf("\nALL TESTS COMPLETE\n");

	
	/*DONE TESTING! WOO!*/
	return 1;
}