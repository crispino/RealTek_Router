#ifndef _LIST_H_
#define _LIST_H_

#include <stdbool.h>

typedef struct node{
    void *data;
    struct node *next;
}node;

typedef struct{ 
    struct node *head;
    struct node *tail;
    long len;
}List;
extern void  list_init(List *list);	
extern bool  is_empty(List *list);
extern void  list_insert(List *list, void *data); 
extern void  list_insert_at_head(List *list, void *data); 
extern void  list_insert_at_tail(List *list, void *data);  
extern void  list_insert_at_index(List *list, void *data, long idx);
extern void *list_delete(List *list, void *key, int (*compare)(const void *, const void *));
extern void *list_search(List *list, void *key, int (*compare)(const void *, const void *));
extern void  list_sort(List *list, int (*compare)(const void *, const void *));
extern void  list_traverse(List *list, void (*handle)(void *));
extern void  list_reverse(List *list);
extern long  list_get_length(List *list);
extern void *list_get_element(List *list, int idx);
extern void  list_destroy(List *list, void (*destroy)(void *data));	

#endif
