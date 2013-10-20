/*Brendan Parks
  CS214 - Assignment 2
  sorted-list.c
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorted-list.h"


/*
 * SLCreate creates a new, empty sorted list.  The caller must provide
 * a comparator function that can be used to order objects that will be
 * kept in the list.
 * 
 * If the function succeeds, it returns a (non-NULL) SortedListT object.
 * Else, it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

void *SLFind(SortedListPtr list, void *newObj) {
	SortedListIteratorPtr iterator = NULL;
	iterator = SLCreateIterator(list);

	ListNode curNode = list->begin;
	if (list->begin == NULL) return NULL;
	if ( list->cf(newObj,  curNode->item) == 0) {
		return curNode->item;
	}
	
	
	while ( (curNode = curNode->next) ) {
		if ( list->cf(newObj,  curNode->item) == 0) {
			return curNode->item;
		}
	}
	
	return NULL;
}
 
SortedListPtr SLCreate(CompareFuncT cf){
	SortedListPtr sl;
	sl = (SortedListPtr) malloc(sizeof(SortedListPtr));
	
	if (sl == NULL) return NULL; /*Unsuccessful*/
	
	sl->begin = NULL;
	sl->cf = cf;
	
	return sl;
	
}


/*
 * SLDestroy destroys a list, freeing all dynamically allocated memory.
 *
 * You need to fill in this function as part of your implementation.
 */
void SLDestroy(SortedListPtr list){
	
	if (list == NULL) return;
	
	ListNode current = NULL;
	current = list->begin;
	
	ListNode t = NULL;
	while (current != NULL) {
		t = current->next;
		free(current->item);
		free(current);
		current = t;
	}
	
	free(list);
}




/*
 * SLInsert inserts a given object into a sorted list, maintaining sorted
 * order of all objects in the list.  If the new object is equal to a subset
 * of existing objects in the list, then the subset can be kept in any
 * order.
 *
 * If the function succeeds, it returns 1.  Else, it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */


ListNode NodeCreate(void *newObj, ListNode next){
		ListNode lnNew = (ListNode)malloc(sizeof(ListNode));
		
		if (lnNew == NULL) return NULL;
		lnNew->item = newObj;
		lnNew->next = next;
		lnNew->refCount = 1;
		
		return lnNew;

}

 /*SLInsert_T is a function that should only be called internally by SLInsert.
	it is used for the recursive cases of insertion.*/
static int SLInsert_T(SortedListPtr list, ListNode curNode, void *newObj){
	/*If traversed to node that doesn't exist, failure occurs*/
	if (curNode == NULL) return 0;
	
	if ( list->cf(newObj,  curNode->item) < 0) {
	
		/*If nowhere else to go, insert at end of list*/
		if (curNode->next == NULL){
			ListNode lnNew = NodeCreate(newObj, curNode->next);
			curNode->next = lnNew;
			return 1;			
		}
		
		/*If new obj is less than current, keep iterating through*/
		return SLInsert_T(list, curNode->next, newObj);
	}
	
	/*At this point, new obj is bigger or equal current node's obj, insert after it*/
	
	ListNode lnNew = NodeCreate(newObj, curNode->next);
	
	/*At this point, it is in list node and pointing to
	  predecessor's sucessor*/
	 
	curNode->next = lnNew;
	
	/*Now predecessor points to new object's list node,
	  and new list node points to the predecessor's previous
	  successor*/
	return 1;
}


 /*-1 if first obj smaller, 0 if equal, 1 if 2nd obj is smaller*/
int SLInsert(SortedListPtr list, void *newObj){

	if (newObj == NULL) return 0; /*Disallow null objects storage*/
	
	ListNode curNode = list->begin;
	if (curNode == NULL) {
		ListNode lnNew = NodeCreate(newObj, list->begin);
		list->begin = lnNew;
		return 1;
	
	} else if ( list->cf(newObj,  curNode->item) > 0) {
		/*New obj is greater than head node, becomes new head node*/

		ListNode lnNew = NodeCreate(newObj, list->begin);
		
		if (lnNew == NULL) return 0;
		
		list->begin = lnNew;
		lnNew->next = curNode;
		/*Now predecessor points to new object's list node,
		  and new list node points to the predecessor's previous
		  successor*/
		return 1;	
	
	
	
	} else{
	
		return SLInsert_T(list, curNode, newObj);
	}
	
		


}



/*
 * SLRemove removes a given object from a sorted list.  Sorted ordering
 * should be maintained.
 *
 * If the function succeeds, it returns 1.  Else, it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */
 
 
 /*SLRemove_T is a function that should only be called internally by SLRemove.
	it is used for the recursive cases of deletion.*/
static ListNode SLRemove_T(SortedListPtr list, ListNode curNode, void *newObj){


	if (curNode == NULL) return NULL; /*Unsuccessful*/
	
	/*Compare with comparator function, then remove*/
	if ( list->cf(newObj,  curNode->item) == 0) {
		/*Found node, remove*/
		
		ListNode lsTemp;
		lsTemp = curNode->next;
		
		if (curNode->refCount == 1){
			free (curNode->item);
			free (curNode);	
		}
		
		return lsTemp;
	}
	
	curNode->next = SLRemove_T(list, curNode->next, newObj);
	return curNode;
	
	
}
 
int SLRemove(SortedListPtr list, void *newObj){

	ListNode curNode = list->begin;	
	if ( list->cf(newObj,  curNode->item) == 0) {
		/*Node to remove is at head*/
		list->begin = curNode->next;
		
		if (curNode->refCount == 1){
		
			free (curNode->item);
			free (curNode);
		}
		
		return 1;
	
	} else {
		if ( SLRemove_T(list, curNode, newObj) == NULL){
			return 0;  /*Unsuccessful*/
		} else return 1;	
	
	}

	

}







/*
 * SLCreateIterator creates an iterator object that will allow the caller
 * to "walk" through the list from beginning to the end using SLNextItem.
 *
 * If the function succeeds, it returns a non-NULL SortedListIterT object.
 * Else, it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

SortedListIteratorPtr SLCreateIterator(SortedListPtr list){

	SortedListIteratorPtr sli;
	sli = (SortedListIteratorPtr)malloc( sizeof (SortedListIteratorPtr) );
	if (sli==NULL) return NULL;
	
	sli->current = list->begin;
	if (sli->current != NULL){
		sli->current->refCount++;
	}

	sli->list = list;
	
	sli->beginIterating = 0;
	return sli;

}


/*
 * SLDestroyIterator destroys an iterator object that was created using
 * SLCreateIterator().  Note that this function should destroy the
 * iterator but should NOT affectt the original list used to create
 * the iterator in any way.
 *
 * You need to fill in this function as part of your implementation.
 */

void SLDestroyIterator(SortedListIteratorPtr iter){
	if (iter != NULL){
		if (iter->current != NULL) {
			if (iter->current->refCount <= 1){
				free(iter->current->item);
				free(iter->current);
			} else {
				iter->current->refCount--;
			}
		}
		
		free (iter);
	
	}

}
 

/*
 * SLNextItem returns the next object in the list encapsulated by the
 * given iterator.  It should return a NULL when the end of the list
 * has been reached.
 *
 * One complication you MUST consider/address is what happens if a
 * sorted list encapsulated within an iterator is modified while that
 * iterator is active.  For example, what if an iterator is "pointing"
 * to some object in the list as the next one to be returned but that
 * object is removed from the list using SLRemove() before SLNextItem()
 * is called.
 *
 * You need to fill in this function as part of your implementation.
 */

void *SLNextItem(SortedListIteratorPtr iter){
	
	/*Needs this to get first node*/
	if (iter->beginIterating == 0) {
		iter->beginIterating = 1;
		if (iter->current == NULL) return NULL;
		return iter->current->item;
	}
	
	if (iter->current->refCount == 1){
		free(iter->current->item);
		free(iter->current);
	} else {
		iter->current->refCount--;
	}
	
	if (iter->current == NULL) return NULL;
	ListNode temp= iter->current->next;
	iter->current = temp;
	if (iter->current == NULL) return NULL;
	
	
	iter->current->refCount++;
	return iter->current->item;
	
}