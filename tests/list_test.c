#include "posixc.h"

typedef struct AA{
    list_node events;
} AA;

typedef struct BB{
    list_node node;
    int x;
} BB;


int main()
{
    AA aa;
    INIT_LIST_HEAD(&aa.events);
    BB b1; 
    INIT_LIST_HEAD(&b1.node);
    b1.x=1;
    BB b2;
    INIT_LIST_HEAD(&b2.node);
    b2.x=2;
    BB b3;
    INIT_LIST_HEAD(&b3.node);
    b3.x=3;
    list_add(&b1.node, &aa.events);
    list_add(&b2.node, &aa.events);
    list_add(&b3.node, &aa.events);
    list_node*tmp;
	for (tmp = aa.events.next; tmp != &aa.events; tmp = tmp->next){
        printf("duh~\n");
    }
    BB* bx;
    list_for_each_entry(bx, &aa.events, node){
	    printf("bx->x=%d\n", bx->x);
    }


    return 0;
}