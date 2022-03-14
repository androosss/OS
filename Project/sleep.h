#ifndef SLEEP_H_
#define SLEEP_H_
#include "kersem.h"
class KernelSem;
class Sleep
{
    public:
    struct slnode{
        slnode* next;
        PCB* val;
        int max;
        int cur;
        slnode(PCB* a,int m,int c)
        {
            next=0;
            val=a;
            max=m;
            cur=0;
        }
    };
    slnode* g;
    slnode* k;
    KernelSem* s;
    Sleep(KernelSem* sem);
    int empty();
    PCB* remove();
    void put(PCB* a,int m,int c);
    void tack();
    ~Sleep();
};

#endif /* SLEEP_H_ */
