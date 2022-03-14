#ifndef LIST_H_
#define LIST_H_
#include "pcb.h"
class PCBlist
{
    public:
    struct node{
        node* next;
        PCB* val;
        node(PCB* a)
        {
            next=0;
            val=a;
        }
    };
    node* g;
    node* k;
    PCBlist();
    int empty();
    PCB* remove();
    void put(PCB* a);
    PCB* get(ID a);
    ~PCBlist();
};
#endif // _LIST_H_
