#include "sorted-list.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * SLCreateItem creates a new list item using a piece of opaque data.
 * If it fails, it returns a null pointer. Otherwise, it returns a 
 * non-null pointer to the item.
 */
ItemPtr SLCreateItem(void *d)
{
	ItemPtr item = malloc(sizeof(struct Item));
	
	if(!item){
		return NULL;
	}
	
	item->refCount = 0;
	item->next = NULL;
	item->data = d;
	
	return item;
}


/*
 * SLDestroyItem destroys a list item, freeing all dynamically allocated memory.
 * It calls the SortedListT's destruct function on the ItemT object's data before
 * finally freeing the ItemT object.
 */
void SLDestroyItem(ItemPtr i, void *dF)
{
	((void(*)(void *))dF)(i->data);
	
	free(i);
	
	return;

}


/*
 * SLCreate creates a new, empty sorted list.  The caller must provide
 * a comparator function that can be used to order objects that will be
 * kept in the list, and a destruct function that gets rid of the objects
 * once they are no longer in the list or referred to in an iterator.
 * 
 * If the function succeeds, it returns a (non-NULL) SortedListT object,
 * otherwise, it returns NULL.
 */
SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df)
{
	SortedListPtr list = malloc(sizeof(struct SortedList));
	
	if(!list){
		return NULL;
	}
	
	list->compFunc = cf;
	list->destFunc = df;
	list->head = NULL;
	
	return list;

}


/*
 * SLDestroy destroys a list, freeing all dynamically allocated memory.
 * It loops through the linked list, calling SLDestroyItem on each ItemT
 * object before finally freeing the SortedListT object.
 */
void SLDestroy(SortedListPtr list)
{
	ItemPtr ptr = list->head;
	ItemPtr tmp = ptr;
	
	while(ptr){
		
		tmp = ptr->next;
		SLDestroyItem(ptr, list->destFunc);
		ptr = tmp;
	}
	
	free(list);
	
	return;

}


/*
 * SLCreateIterator creates an iterator object that will allow the caller
 * to "walk" through the list from beginning to the end using SLNextItem.
 *
 * If the function succeeds, it returns a non-NULL pointer to a
 * SortedListIterT object, otherwise it returns NULL.  The SortedListT
 * object should point to the first item in the sorted list, if the sorted
 * list is not empty.  If the sorted list object is empty, then the iterator
 * should have a null pointer.
 */
SortedListIteratorPtr SLCreateIterator(SortedListPtr list)
{
	
	if(!list){
		return NULL;
	}
	
	SortedListIteratorPtr iter = malloc(sizeof(struct SortedListIterator));
	
	if(!iter){
		return NULL;
	}
	
	iter->item = list->head;		//If the sorted list is empty, list.head will be null.
	
	ItemPtr tmp = list->head;
	
	if(tmp){
		tmp->refCount++;
	}
	
	iter->destFunc = list->destFunc;
	
	return iter;

}


/*
 * SLDestroyIterator destroys an iterator object that was created using
 * SLCreateIterator().  Note that this function should destroy the
 * iterator but should NOT affect the original list used to create
 * the iterator in any way.
 */
void SLDestroyIterator(SortedListIteratorPtr iter)
{
	free(iter);
	
	return;

}


/*
 * SLGetItem returns the pointer to the data associated with the
 * SortedListIteratorPtr.  It should return 0 if the iterator
 * advances past the end of the sorted list.
*/
void * SLGetItem( SortedListIteratorPtr iter )
{
	if(iter->item == NULL){
		return 0;
	}
	
	return iter->item->data;

}


/*
 * SLNextItem returns the pointer to the data associated with the
 * next object in the list encapsulated by the given iterator.
 * It should return a NULL when the end of the list has been reached.
 *
 * One complication you MUST consider/address is what happens if a
 * sorted list encapsulated within an iterator is modified while that
 * iterator is active.  For example, what if an iterator is "pointing"
 * to some object in the list as the next one to be returned but that
 * object is removed from the list using SLRemove() before SLNextItem()
 * is called.
 *
 * SLNextItem advances the iterator before returning data. If the ItemT
 * object has been removed from the list, it will still be pointing to the
 * appropriate object, so the iterator will simply advance.
 */
void * SLNextItem(SortedListIteratorPtr iter)
{
	if(iter->item == NULL){
		return NULL;
	}
	
	ItemPtr prev = iter->item;
	
	iter->item = iter->item->next;
	prev->refCount--;
	
	if(iter->item){
	iter->item->refCount++;
	}
	
	if(prev->refCount == 0){
		SLDestroyItem(prev, iter->destFunc);
	}
	
	if(iter->item == NULL){
		return NULL;
	}
	
	return SLGetItem(iter);

}


/*
 * SLInsert inserts a given object into a sorted list, maintaining sorted
 * order of all objects in the list.  If the new object is equal to a subset
 * of existing objects in the list, then the subset can be kept in any
 * order.
 *
 * If the function succeeds, it returns 1, otherwise it returns 0.
 */
int SLInsert(SortedListPtr list, void *newObj){
	
	if(list == NULL || newObj == NULL){
		return 0;
	}
	
    ItemPtr node = SLCreateItem(newObj);

    //check if it's a head
    if(list->head == NULL){
        list->head = node;
        node->refCount = 1;
        return 1;
    }
	
	//iterate through linked list
	ItemPtr tmp = list->head;
	ItemPtr prev = list->head;
	
	/* Only need to increment refCount for persistent (non-temporary) pointers */
	//tmp->refCount =+ 2;
	
	int result = ((int(*)(void *, void *))list->compFunc)(tmp->data, node->data);
	
	/* node IS the new head. insertion at the beginning of the list */
	if(result == -1){
		node->next = tmp;
		list->head = node;
		node->refCount = 1;
		return 1;
	}else if(result == 0){
		SLDestroyItem(node, list->destFunc);
		return 0;
	}
	
	tmp = tmp->next;
	
	if(tmp == NULL)
	{
		prev->next = node;
		node->refCount = 1;
		return 1;
	}
	
	/* node is NOT the new head. loop through the list */
	while(tmp->next){

		result =  ((int(*)(void *, void *))list->compFunc)(tmp->data, node->data);
			
		/* node is smaller than tmp */
		if(result == 1){
			prev = tmp;
			tmp = tmp->next;
			continue;
				
		/* node is greater than tmp. insert node */
		}else if(result == -1){
			node->next = tmp;
			prev->next = node;
			node->refCount = 1;
			return 1;
		/* duplicate */
		}else{
			SLDestroyItem(node, list->destFunc);
			return 0;
		}
	}
        result =  ((int(*)(void *, void *))list->compFunc)(tmp->data,node->data);
        if(result == 1){
			//insert at the end
			tmp->next=node;
			node->refCount = 1;
			return 1;
			
		/* duplicate */
		}else if(result == 0){
			SLDestroyItem(node, list->destFunc);
			return 0;
		}else{
			node->next = tmp;
			prev->next = node;
			node->refCount = 1;
			return 1;
		}

}


/*
 * SLRemove removes a given object from a sorted list.  Sorted ordering
 * should be maintained.  SLRemove may not change the object whose
 * pointer is passed as the second argument.  This allows you to pass
 * a pointer to a temp object equal to the object in the sorted list you
 * want to remove.
 *
 * If the function succeeds, it returns 1, otherwise it returns 0.
 */
int SLRemove(SortedListPtr list, void *newObj){
    
	if(list == NULL || newObj == NULL){
		return 0;
	}
	
	/*if list is empty, return 0 */
	if(list->head == NULL){
		return 0;
	}
	
	ItemPtr tmp = list->head;
	ItemPtr prev = tmp;
	
    int result = ((int(*)(void *, void *))list->compFunc)(tmp->data, newObj);
	
    if (result == 0){ /*remove head*/
        list->head = tmp->next;
		tmp->refCount--;
		list->head->refCount++;
		
		if(tmp->refCount == 0){
        SLDestroyItem(tmp, list->destFunc);
		list->head->refCount--;
		}
        return 1;
    }
    else{
        tmp = tmp->next;
    }
	
	while(tmp){
		result = ((int(*)(void *, void *))list->compFunc)(tmp->data, newObj);
		
		if(result == 0){
			prev->next = tmp->next;
			tmp->refCount--;
			if(tmp->next){
				tmp->next->refCount++;
			}
			
			if(tmp->refCount == 0){
				if(tmp->next){
					tmp->next->refCount--;
				}
				SLDestroyItem(tmp, list->destFunc);
			}
			return 1;
		}else if(result == -1){
			break;
		}
		
		prev = tmp;
		tmp = tmp->next;
	}
	
	return 0;

}
