#ifndef SLIST_H_
#define SLIST_H_
#include "kersem.h"
class KernelSem;
class slist
{
    public:
    struct snode{
        snode* next;
        KernelSem* val;
        snode(KernelSem* a)
        {
            next=0;
            val=a;
        }
    };
    snode* g;
    snode* k;
    slist();
    int empty();
    void put(KernelSem* a);
    void notify();
    int size();
    ~slist();
};




#endif /* SLIST_H_ */
