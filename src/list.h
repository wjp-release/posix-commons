#pragma once

// linux kernel list

/*
Init
[1] 
	struct kool_list mylist;
	INIT_LIST_HEAD(&mylist.list);
[2]
    LIST_HEAD(mylist);

*/


/*
Example

#include <stdio.h>
#include <stdlib.h>

#include "list.h"

struct kool_list{
	int to;
	struct list_head list;
	int from;
};

int main(int argc, char **argv){
	struct kool_list *tmp;
	struct list_head *pos, *q;
	unsigned int i;

	struct kool_list mylist;
	INIT_LIST_HEAD(&mylist.list);

	for(i=5; i!=0; --i){
		tmp= (struct kool_list *)malloc(sizeof(struct kool_list));
		
		printf("enter to and from:");
		scanf("%d %d", &tmp->to, &tmp->from);

		list_add(&(tmp->list), &(mylist.list));
	}
	printf("\ntraversing the list using list_for_each()\n");
	list_for_each(pos, &mylist.list){
		 tmp= list_entry(pos, struct kool_list, list);
		 printf("to= %d from= %d\n", tmp->to, tmp->from);
	}

	list_for_each_entry(tmp, &mylist.list, list)
		 printf("to= %d from= %d\n", tmp->to, tmp->from);

	list_for_each_safe(pos, q, &mylist.list){
		 tmp= list_entry(pos, struct kool_list, list);
		 list_del(pos);
		 free(tmp);
	}
	return 0;
}

*/

typedef struct list_head {
	struct list_head *next, *prev;
}list_head;

typedef struct list_head list_node;  

#define INIT_LIST_HEAD(ptr) do { \
	(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)


static inline void __list_add(struct list_head *cur,
			      struct list_head *prev,
			      struct list_head *next)
{
	next->prev = cur;
	cur->next = next;
	cur->prev = prev;
	prev->next = cur;
}

static inline void list_add(struct list_head *cur, struct list_head *head)
{
	__list_add(cur, head, head->next);
}

static inline void list_add_tail(struct list_head *cur, struct list_head *head)
{
	__list_add(cur, head->prev, head);
}

static inline void __list_del(struct list_head *prev, struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = (void *) 0;
	entry->prev = (void *) 0;
}

static inline void list_del_init(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry); 
}

static inline void list_move(struct list_head *list, struct list_head *head)
{
        __list_del(list->prev, list->next);
        list_add(list, head);
}

static inline void list_move_tail(struct list_head *list,
				  struct list_head *head)
{
        __list_del(list->prev, list->next);
        list_add_tail(list, head);
}

static inline int list_empty(struct list_head *head)
{
	return head->next == head;
}

static inline void __list_splice(struct list_head *list,
				 struct list_head *head)
{
	struct list_head *first = list->next;
	struct list_head *last = list->prev;
	struct list_head *at = head->next;

	first->prev = head;
	head->next = first;

	last->next = at;
	at->prev = last;
}

static inline void list_splice(struct list_head *list, struct list_head *head)
{
	if (!list_empty(list))
		__list_splice(list, head);
}

static inline void list_splice_init(struct list_head *list,
				    struct list_head *head)
{
	if (!list_empty(list)) {
		__list_splice(list, head);
		INIT_LIST_HEAD(list);
	}
}

#define list_entry(ptr, type, member) \
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))


#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); \
        	pos = pos->next)

#define list_for_each_prev(pos, head) \
	for (pos = (head)->prev; pos != (head); \
        	pos = pos->prev)
        	

#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)


#define list_for_each_entry(pos, head, member)				\
	for (pos = list_entry((head)->next, typeof(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = list_entry(pos->member.next, typeof(*pos), member))

#define list_for_each_entry_safe(pos, n, head, member)			\
	for (pos = list_entry((head)->next, typeof(*pos), member),	\
		n = list_entry(pos->member.next, typeof(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = n, n = list_entry(n->member.next, typeof(*n), member))

