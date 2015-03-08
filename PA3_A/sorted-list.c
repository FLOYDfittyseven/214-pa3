#include <stdio.h>
#include <stdlib.h>
#include "sorted-list.h"

/* sorted-list.c
    implemented by:
    Anush Pogosyan ap7879
    Jarrett Mead jfm168

*/
typedef struct SortedList* SortedListPtr;
typedef struct SortedListIterator* SortedListIteratorPtr;
//typedef void (*DestructFuncT)( int * ab);
void SLDestroyIterator(SortedListIteratorPtr iter);
//int * SLGetItem( SortedListIteratorPtr iter );
//int * SLNextItem(SortedListIteratorPtr iter);
void SLDestroy(SortedListPtr list);
//typedef int (*CompareFuncT)( int *a, void *b);

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df);
int SLInsert(SortedListPtr list, void *newObj);
int SLRemove(SortedListPtr list, void *newObj);
typedef struct node nodeS;
SortedListIteratorPtr SLCreateIterator(SortedListPtr list);

/*struct node {
    int referenceCount;
    nodeS *next;
    int *data;
};

struct SortedList{
};

 struct SortedListIterator{
    nodeS *thisItem;
};
*/

SortedListIteratorPtr SLCreateIterator(SortedListPtr list){
    if(!list) {
            return NULL;
    }
    SortedListIteratorPtr iter = (SortedListIteratorPtr)malloc(sizeof(struct SortedListIterator));
    if(!iter){
        return NULL;
    }
    iter->thisItem = list->head;
    if(iter->thisItem != NULL) iter->thisItem->referenceCount++;
    return iter;
}

void SLDestroyIterator(SortedListIteratorPtr iter)
{
    if(iter != NULL){
        iter->thisItem->referenceCount--;
        if(iter->thisItem->referenceCount == 0){ /* iterator is the only thing pointing to the object -> free the memory allocated for the object before destroying iterator*/
            if(iter->thisItem->next != NULL){
                iter->thisItem->next->referenceCount--;
                iter->thisItem->next= NULL;
            }
            //list->DestructFunc(iter->thisItem);
            free(iter->thisItem->data); /*destruct function on data*/
            free(iter->thisItem);
        }
        free(iter);
    }
}
/*
 * SLGetItem returns the pointer to the data associated with the
 * SortedListIteratorPtr.  It should return 0 if the iterator
 * advances past the end of the sorted list.
 *
 * You need to fill in this function as part of your implementation.
*/
void * SLGetItem( SortedListIteratorPtr iter ){
    if(iter->thisItem != NULL){
        return iter->thisItem->data;
    }
    printf("there are no more items in this list\n");
    return 0;
}

void * SLNextItem(SortedListIteratorPtr iter){
    iter->thisItem->referenceCount--;
    if(iter->thisItem->referenceCount != 0){
        iter->thisItem = iter->thisItem->next;
    }
    else{ /*if node was removed while having this iterator on it*/
        /*destruct */
        SortedListIteratorPtr tmp = iter;
        iter->thisItem = iter->thisItem->next;
        free(tmp);  /*the object who was pointed only by iterator is removed */
    }
    if(iter->thisItem == NULL){
        printf("list has reached its end\n");
        return NULL;
    }
    iter->thisItem->referenceCount++;
    return iter->thisItem->data;
}
/*
 * SLCreate creates a new, empty sorted list.  The caller must provide
 * a comparator function that can be used to order objects that will be
 * kept in the list, and a destruct function that gets rid of the objects
 * once they are no longer in the list or referred to in an iterator.
 * If the function succeeds, it returns a (non-NULL) SortedListT object,
 * otherwise, it returns NULL.
 */

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df)

{
	SortedListPtr list = (SortedListPtr) malloc(sizeof(struct SortedList));
	if(!list){
		return NULL;
	}
	if(cf != NULL && df != NULL){
        list->CompareFunc = cf;
        list->DestructFunc = df;
        list->head = NULL;
        return list;
	}
	return NULL;
}


void SLDestroy(SortedListPtr list){
    printf("EXECUTE ORDER 66\n");
    if(list){
        nodeS *tmp, *next;
        tmp = list->head;
        next = NULL;
        free(list);
        while(tmp->next != NULL){
            next= tmp->next;
            tmp->referenceCount--;
            if(tmp->referenceCount==0){
            list->DestructFunc(tmp->data);
            free(tmp);
            }
        }
        if(tmp->next == NULL){
            if(tmp->referenceCount==0){
            list->DestructFunc(tmp->data);
            free(tmp);
            }
        }
    }
}
int CompareF(int *a, void *b)
{
  int *da =  a;
  int *db = (int *) b;
  if(*da > *db){
     return 1;
  }
  else if(*da < *db){
    return -1;
  }
  else {
     return 0;
  }
}
void DestructF ( int * ab){
    printf("removing value\t");
    *ab = 0;
    printf("%d\n", *ab);
}



int SLInsert(SortedListPtr list, void *newObj){
    if(list == NULL || newObj == NULL){
		return 0;
	}
    nodeS *node = (nodeS *)malloc(sizeof(nodeS));
    node->next = NULL;
    node->data = newObj;
    /*insert as first element*/
    if(list->head == NULL){
        list->head=node;
        list->head->referenceCount++;
        return 1;
    }
    else{
        //iterator through linked list
        nodeS *tmp = list->head;
        nodeS *prev = NULL;
        int result =  list->CompareFunc(tmp->data,node->data);
        /*insertiona at the beginning of the list*/
        if(result == -1){
            node->next = tmp;
            list->head= node;
            list->head->referenceCount++;
            return 1;
        }
        while(tmp->next != NULL){
            result =  list->CompareFunc(tmp->data,node->data);
            /*iterating through list while value is greater*/
            if(result == 1){
                prev = tmp;
                tmp = tmp->next;
                continue;
            }
            if(result == -1){
                /*the place of the value is right befure the current one */
                node->next=tmp;
                prev->next=node;
                node->referenceCount++;
                return 1;
            }
             /* objects are equal*/
            list->DestructFunc(node->data);
            free(node);
            return 0;
        }
        result =  list->CompareFunc(tmp->data,node->data);
        if(result == 1){
                /*inset at the end*/
                tmp->next=node;
                node->referenceCount++;
                return 1;
        }
        /*add equals check and destroy node */
        if(result == 0){
            list->DestructFunc(node->data);
            free(node);
            return 0;
        }
        /*check for the last */
        node->next = tmp;
        prev->next = node;
        node->referenceCount++;
        return 1;
    }

}

int SLRemove(SortedListPtr list, void *newObj){
    if(list== NULL) return 0; /*list is empty*/
    int result;
    nodeS *tmp = list->head;
    nodeS *prev = NULL;
    result = list->CompareFunc(tmp->data,newObj);
    if (result == 0){ /*remove from head*/

        if(list->head->next == NULL){ /*list had only one element, free the memory, unless iterator is pointing */
                tmp->referenceCount--;
                if(tmp->referenceCount == 0){
                    list->DestructFunc(tmp->data);
                    free(tmp);
                }
                else{
                    list->head=NULL;
                }
                printf("head is removed, list is empty\n");
                return 1;
        }
        /*remove the head*/
        list->head = tmp->next;
        tmp->next->referenceCount++;
        tmp->referenceCount--;

    }
    else if(result == -1){
        printf("object not in the list\n");
        return 0;
    }
    else{
        if(list->head->next == NULL){
            printf("object not in the list\n");
            return 0;
        }
        prev= tmp;
        tmp = tmp->next;
        while (tmp->next != NULL){
            result = list->CompareFunc(tmp->data,newObj);
            if(result == 0){
                prev->next=tmp->next;
                prev->next->referenceCount++;
                tmp->referenceCount--;
                break;
          }
            else if(result == 1){
                prev = tmp;
                tmp = tmp->next;
            }
            else{
                 printf("object not in the list\n");
                 return 0;
            }
        }
        /* if tmp-> next == NULL */
        if(tmp-> next == NULL){
            result = list->CompareFunc(tmp->data,newObj);
            if(result == 0){
                prev->next=NULL;
                tmp->referenceCount--;
                if(tmp->referenceCount==0){  /*no iterator is pointing*/
                    list->DestructFunc(tmp->data);
                    free(tmp);
                }
                return 1;
            }
            else{
                printf("object is not in the list\n");
                return 0;
            }
        }
    }
    if(tmp->referenceCount==0){  /*no iterator is pointing*/
    list->DestructFunc(tmp->data);
    tmp->next->referenceCount--;
    free(tmp);
    return 1;
    }
    return 0;
}






