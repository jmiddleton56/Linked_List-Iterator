//
// Iterator and all related code developed by Joel Middleton on 2023-10-11.
// My contributions begin on line 163.
// Linked_List (Up to Line 160) Created by Alex Brodsky on 2023-09-20.
//  
  
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "linkedlist.h"
#include <string.h>

typedef struct ll_node {
    struct ll_node *prev;
    struct ll_node *next;
    void *item;
} ll_node_t;

typedef struct linked_list {
    ll_node_t *head;
    ll_node_t *tail;
} linked_list;

extern linked_list_t ll_new() {
    linked_list_t * list = calloc(1,sizeof(linked_list));
    return list;
}

extern void ll_destroy(linked_list_t ll) {
    linked_list  *list = (linked_list *)ll;
    assert(list != NULL);

    while (list->head != NULL) {
        ll_node_t *tmp = list->head;
        list->head = list->head->next;
        free(tmp);
    }

    free(list);
}

static ll_node_t *new_node(void *item, ll_node_t *prev, ll_node_t *next) {
    ll_node_t *n = calloc(1, sizeof(ll_node_t));
    if (n != NULL) {
        n->item = item;
        n->prev = prev;
        n->next = next;
    }
    return n;
}


extern int ll_add_front(linked_list_t ll, void *item) {
    linked_list  *list = (linked_list *)ll;
    assert(list);

    ll_node_t *n = new_node(item,NULL, list->head);
    if (n != NULL) {
        if (list->tail == NULL) {
            list->tail = n;
        } else {
            list->head->prev = n;
        }
        list->head = n;
    }
    return n != NULL;
}

extern int ll_add_back(linked_list_t ll, void *item) {
    linked_list  *list = (linked_list *)ll;
    assert(list);

    ll_node_t *n = new_node(item,list->tail, NULL);
    if (n != NULL) {
        if (list->head == NULL) {
            list->head = n;
        } else {
            list->tail->next = n;
        }
        list->tail = n;
    }
    return n != NULL;
}

extern void * ll_remove_front(linked_list_t ll) {
    linked_list  *list = (linked_list *)ll;
    assert(list);

    void *item = NULL;
    if (list->head != NULL) {
        ll_node_t *n = list->head;
        list->head = list->head->next;
        if (list->head == NULL) {
            list->tail = NULL;
        } else {
            list->head->prev = NULL;
        }

        item = n->item;
        free(n);
    }
    return item;
}

extern void * ll_remove_back(linked_list_t ll) {
    linked_list  *list = (linked_list *)ll;
    assert(list);
    
    void *item = NULL;
    if (list->tail != NULL) {
        ll_node_t *n = list->tail;
        list->tail = list->tail->prev;
        if (list->tail == NULL) {
            list->head = NULL;
        } else {
            list->tail->next = NULL;
        }

        item = n->item;
        free(n);
    }
    return item;
}

extern void * ll_get_front(linked_list_t ll) {
    linked_list  *list = (linked_list *)ll;
    assert(list);

    void *item = NULL;
    if (list->head != NULL) {
        item = list->head->item;
    }
    return item;
}

extern void * ll_get_back(linked_list_t ll) {
    linked_list  *list = (linked_list *)ll;
    assert(list);

    void *item = NULL;
    if (list->head != NULL) {
        item = list->tail->item;
    }
    return item;
}

extern int ll_size(linked_list_t ll) {
    linked_list  *list = (linked_list *)ll;
    assert(list);
    int count = 0;
    for (ll_node_t *ptr = list->head; ptr != NULL; ptr = ptr->next) {
        count++;
    }
    return count;
}

extern int ll_is_empty(linked_list_t ll) {
    linked_list  *list = (linked_list *)ll;
    assert(list);
    return list->head == NULL;
}

/************************************************************************
 * Joel Middleton's code below
 ************************************************************************/

typedef struct iterator {
    linked_list *list;
    ll_node_t *current;
} ll_iterator;

extern ll_iterator_t  ll_iterator_new(linked_list_t list) {
    ll_iterator* iterator = malloc(sizeof(ll_iterator));
    linked_list  *ll = (linked_list *)list;
    if (iterator == NULL) {
        return NULL;
    }

    iterator->list = ll;
    iterator->current = NULL;
//    iter->current->next = ll->head;
    return iterator;
}

extern void ll_iterator_destroy(ll_iterator_t iter) {
    ll_iterator * iterator = (ll_iterator *)iter;
    assert(iterator != NULL);

    while (iterator->current != NULL) {
        ll_node_t *tmp = iterator->current;
        iterator->current = iterator->current->next;
        free(tmp);
    }

    free(iterator);
}

extern void *ll_next(ll_iterator_t iter) {
    ll_iterator * iterator = (ll_iterator*) iter;
    if(iterator->current == NULL) {
        iterator->current = iterator->list->head;
        return iterator->current->item;
    } else if (iterator->current != NULL) {
        iterator->current = iterator->current->next;
        return iterator->current->item;
    }
}

extern int ll_has_next(ll_iterator_t iter) {
    ll_iterator * iterator = (ll_iterator*) iter;
    //Check if current still points to NULL before the first element, and if our list only contains 1 item
    if (iterator->current == NULL && iterator->list->head == iterator->list->tail && iterator->list->head !=NULL) {
        //Return 1 as the next function would return an element (the only element in the list)
        return 1;
    }
    //Check if the list is empty and current points to NULL
    else if (iterator->current == NULL && iterator->list->head == NULL && iterator->list->tail == NULL) { //Check if the list is empty and current points to NULL
        return 0;
    }
    //Otherwise, if current points to before the first element of a non-empty list of size > 1 element
    else if (iterator->current == NULL && (iterator->list->head && iterator->list->tail) != NULL) {
        return 1;
    }
    if ((iterator->current->next) != NULL) {
        return 1;
    } else {
        return 0;
    }
}

extern int ll_add(ll_iterator_t iter, void *item) {
    ll_iterator * iterator = (ll_iterator*) iter;
    linked_list_t  *list = (linked_list_t *)iterator->list;
    //If the list is empty, use ll_add_front to add item to list
    if (iterator->list->head == iterator->list->tail) {
        ll_add_front(list, item);
    }
    //If element is to be inserted before head (at the front of the list)
    else if (iterator->current == iterator->list->head) {
        ll_node_t *newNode = new_node(item, iterator->current, iterator->current->next);
        if (newNode == NULL) {
            return 0;
        }
        iterator->current->next->prev = newNode;
        iterator->current->next = newNode;
        iterator->current = newNode;
    }
        else {
        ll_node_t *newNode = new_node(item, iterator->current, iterator->current->next);
        if (newNode == NULL) {
            return 0;
        }
        iterator->current->next->prev = newNode;
        iterator->current->next = newNode;
        iterator->current = newNode;
    }
        return 1;
}

extern void *ll_set(ll_iterator_t iter, void *item) {
    ll_iterator * iterator = (ll_iterator*) iter;
    ll_node_t *tmp= iterator->current;
    //If replacing head
    if (iterator->current == iterator->list->head) {
        ll_node_t *newNode = new_node(item, NULL, iterator->current->next);
        iterator->current->next->prev = newNode;
        iterator->current->next = NULL;
        iterator->current = newNode;
    }
    //if replacing tail
    else if ( iterator->current == iterator->list->tail) {
        ll_node_t *newNode = new_node(item, iterator->current->prev, NULL);
        iterator->current->prev->next = newNode;
        iterator->current->prev = NULL;
        iterator->current = newNode;
    }
    else {
        ll_node_t *newNode = new_node(item, iterator->current->prev, iterator->current->next);
        iterator->current->prev->next = newNode;
        iterator->current->prev = NULL;
        iterator->current->next->prev = newNode;
        iterator->current->next = NULL;
        iterator->current = newNode;
    }
    if (tmp->item != NULL && tmp->item != iterator->current->item) {
        return tmp->item;
    } else {
        return NULL;
    }
}

extern void *ll_remove(ll_iterator_t iter){
    ll_iterator * iterator = (ll_iterator*) iter;
    linked_list_t  *list = (linked_list_t *)iterator->list;
    void *item = NULL;
    ll_node_t *tmp= iterator->current->prev;
    //If removing head
    if (iterator->current == iterator->list->head) {
        item = ll_remove_front(list);
    }
        //if removing tail
    else if ( iterator->current == iterator->list->tail) {
        item = ll_remove_back(list);
    }
    else {
        /*
         * Removing element from somewhere in the middle, we set the two neighbour nodes to link to one another,
         * remove the current node's links and move our current pointer.
         */
        item = iterator->current->item;
        iterator->current->prev->next = iterator->current->next;
        iterator->current->next->prev = iterator->current->prev;
        iterator->current->prev = NULL;
        iterator->current->next = NULL;
        iterator->current = tmp;
    }
    return item;
}
