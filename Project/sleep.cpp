#include "Sleep.h"
Sleep::Sleep(KernelSem* sem)
{
	g=0;
	k=0;
	s=sem;
}
void Sleep::put(PCB* a,int m,int c)
    {
        if(g==0)
        {
            slnode* p=new slnode(a,m,c);
            g=k=p;
            return;
        }
        if(g!=k)
        {
            slnode *p =new slnode(a,m,c);
            k->next=p;
            k=p;
            return;
        }
        slnode *p =new slnode(a,m,c);
        k->next=p;
        g->next=p;
        k=p;
        return;
}
PCB* Sleep::remove()
    {
    	if(!g) return 0;
    	slnode* p=g;
    	g=g->next;
    	if(!g) k=0;
    	return p->val;
    }
int Sleep::empty()
    {
    	if(g) return 0;
    	else return 1;
    }
Sleep::~Sleep()
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
    while(g!=0)
    {
        slnode* p=g;
        p->val->waitToComplete();
        g=g->next;
        delete p;
    }
    if (g) delete g;
    if (k) delete k;
    if(a) System::unlock();
}
void Sleep::tack()
{
	slnode* p=g;
	while(p)
	{
		p->cur++;
		p=p->next;
	}
	p=g;
	if(!p) return;
	while(p->cur==p->max && p->max!=0)
	{
		g=g->next;
		if(!g) k=0;
		p->val->SemFlag=0;
		if(p->val->state!=4)
		{
			p->val->state=1;
			Scheduler::put(p->val);
		}
		s->value++;
		p=g;
		if(!p) return;
	}
	while(p->next)
	{
		if(p->next->cur==p->next->max && p->next->max!=0)
		{
			if(p->next==k) k=p;
			p->next->val->SemFlag=0;
			if(p->next->val->state!=4)
			{
				p->next->val->state=1;
				Scheduler::put(p->next->val);
			}
			s->value++;
			p->next=p->next->next;
		}
		else p=p->next;
	}
}
