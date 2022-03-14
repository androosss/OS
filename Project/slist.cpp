#include "slist.h"
slist::slist()
{
	g=0;
	k=0;
}
void slist::put(KernelSem* a)
    {
        if(g==0)
        {
            snode* p=new snode(a);
            g=k=p;
            return;
        }
        if(g!=k)
        {
            snode *p =new snode(a);
            k->next=p;
            k=p;
            return;
        }
        snode *p =new snode(a);
        k->next=p;
        g->next=p;
        k=p;
        return;
}
int slist::empty()
    {
    	if(g) return 0;
    	else return 1;
    }
slist::~slist()
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
    while(g!=0)
    {
        snode* p=g;
        g=g->next;
        delete p;
    }
    if (g) delete g;
    if (k) delete k;
    if(a) System::unlock();
}
void slist::notify()
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
	snode* p=g;
	while(p!=0)
	{
		p->val->tack();
		p=p->next;
	}
	if(a) System::unlock();
}
int slist::size()
{
	int a=0;
	snode* p=g;
	while(p)
	{
		a++;
		p=p->next;
	}
	return a;
}
