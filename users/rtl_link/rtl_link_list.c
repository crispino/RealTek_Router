#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "rtl_link_list.h"

void list_init(List *list)
{
	list->head = NULL;
	list->tail = NULL;
	list->len = 0;
}

bool is_empty(List *list)
{
	return (list->head == NULL);
}

static struct node *make_node(void *data)
{
	struct node *n;

	n = malloc(sizeof(struct node));
	assert(n != NULL);

	n->next = NULL;
	n->data = data;

	return n;
}

void list_insert_at_head(List *list, void *data)
{
	struct node *n;
	n = make_node(data);

	if(list->head == NULL){
		list->head = n;
		list->tail = n;
	}
	else{                   
		n->next = list->head;
		list->head = n;
	}
	list->len++;
}

void list_insert_at_index(List *list, void *data, long index)
{
	long i = 1;
	struct node *p, *n;

	p = list->head;

	while(p && i < index){
		p = p->next;
		i++;
	}

	if(p){ 
		n = make_node(data);
		n->next = p->next;
		p->next = n;
		list->len++;
	}
}

void list_insert_at_tail(List *list, void *data)
{
	struct node *n;
	n = make_node(data);

	if(is_empty(list)){ 
		list->head = n;
		list->tail = n;
	}
	else{       
		list->tail->next = n;
		list->tail = n;
	}
	list->len++;
}

void list_insert(List *list, void *data)
{
#if 1
	list_insert_at_tail(list, data);
#else
	struct node *n;

	n = make_node(data);

	if(list->head == NULL){
		list->head = n;
		list->tail = n;
	} else {
		list->tail->next = n;
		list->tail = n;
	}
	list->len++;
#endif
}

void * list_delete(List *list, void *key, int (*compare)(const void *, const void *))
{
	void *data;
	struct node *n, *t;
	n = list->head;

	if(n==NULL)
	    return NULL;

	if(!compare(n->data, key)){
		t = n;
		data = n->data;
		list->head = n->next;
		free(t);
		list->len--;
		return data;
	}

	while(n->next != NULL){
		if(compare(n->next->data, key) == 0){ 
			t = n->next;
			if(n->next == list->tail)
				list->tail = n;
			n->next = n->next->next;
			data = t->data;
			free(t);
			list->len--;
			return data; 
		}
		n = n->next;
	}
	return NULL; 
}

void *list_search(List *list, void *key, int (*compare)(const void *, const void *))
{
	struct node *n;
	n = list->head;

	while(n){
		if(!compare(n->data, key)) 
			return n->data;
		n = n->next;    
	}

	return NULL; 
}

static struct node *find_min_node(List *list,
				  int (*compare)(const void *, const void *))
{
	struct node *min, *n;

	n = list->head;
	min = list->head;

	while(n){
		if(compare(min->data, n->data) > 0)
			min = n;
		n = n->next;
	}

	return min;
}

static void delete_node(List *list, struct node *key)
{
	struct node *n;

	n = list->head;

	if(n == key){
		list->head = n->next;
		return;
	}

	while(n->next){
		if(n->next == key){
			if(key == list->tail)
				list->tail = n;
			n->next = n->next->next;
			return;
		}
		n = n->next;
	}
}

static void insert_node(List *list, struct node *key)
{
	if(list->head == NULL){
		list->head = key;
		list->tail = key;
	}else{
		list->tail->next = key;
		list->tail = key;
	}
}

void list_sort(List *list,
	       int (*compare)(const void *, const void *))
{
	List tmp;
	struct node *n;

	list_init(&tmp);

	while(! is_empty(list)){
		n = find_min_node(list, compare);
		delete_node(list, n);
		n->next = NULL;
		insert_node(&tmp, n);
	}
	list->head = tmp.head;
	list->tail = tmp.tail;
}

void list_traverse(List *list, void (*handle)(void *))
{
	struct node *p;
	p = list->head;

	while(p){
		handle(p->data);
		p = p->next;
	}
}

void list_reverse(List *list)
{
	struct node *pre = NULL, *next, *p = list->head;

	list->tail = list->head;  
	while(p){
		next = p->next;
		if(!next) 
			list->head = p;
		
		p->next = pre;
		pre = p;
		p = next;
	}
}

long list_get_length(List *list)
{
	return (list->len);
}

void *list_get_element(List *list, int idx)
{
	int i = 0;
	struct node *n;
	n = list->head;

	while(n && i < idx){
		i++;
		n = n->next;
	}

	if(n)
		return n->data;

	return NULL;
}

void list_destroy(List *list, void (*destroy)(void *))
{
	struct node *n, *t;
	n = list->head;

	while(n){
		t = n->next; 
		if(destroy)  
			destroy(n->data);
		free(n);    
		n = t; 
	}

	list->head = NULL;
	list->tail = NULL;
	list->len = 0;
}
