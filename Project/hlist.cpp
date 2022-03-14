#include "hlist.h"

HandlerList::HandlerList()
{
	g=k=0;

}
void HandlerList::put(SignalHandler h)
{
	hnode* p=new hnode(h);
	if(!g)
	{
		g=p;
		k=p;
		return;
	}
	if(g==k)
	{
		g->next=p;
		k=p;
		return;
	}
	k->next=p;
	k=k->next;
}
void HandlerList::swap(SignalHandler h1,SignalHandler h2)
{
	hnode* p1;
	while(p1)
	{
		if(p1->hand==h1)
			break;
		p1=p1->next;
	}
	hnode* p2;
	while(p2)
	{
		if(p2->hand==h1)
			break;
		p2=p2->next;
	}
	if(p1 && p2)
	{
		SignalHandler temp=p1->hand;
		p1->hand=p2->hand;
		p2->hand=temp;
		temp=0;
	}
}
void HandlerList::callHandlers()
{
	hnode* p=g;
	while(p)
	{
		p->hand();
		p=p->next;
	}
}
void HandlerList::copy(HandlerList* list)
{
	hnode* p=list->g;
	while(p)
	{
		put(p->hand);
		p=p->next;
	}
}
HandlerList::~HandlerList()
{
	while(g)
	{
		hnode* p=g;
		g=g->next;
		delete p;
	}
	g=0;
	k=0;
}
SignalList::SignalList()
{
	g=0;
	k=0;
}
void SignalList::put(int s)
{
	snode* p=new snode(s);
	if(!g)
	{
		g=p;
		k=p;
		return;
	}
	if(g==k)
	{
		g->next=p;
		k=p;
		return;
	}
	k->next=p;
	k=k->next;
}
void SignalList::SignalAll()
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
	asm sti;
	snode* p=g;
	if(!g) {asm cli; if(a) System::unlock(); return;}
	while(PCB::globalSignalState[g->sig] && PCB::running->localSignalState[g->sig])
	{
		//printf("\nobradjuje se signal %d nit %d\n",g->sig,PCB::getPRunningId());
		PCB::running->handlers[g->sig]->callHandlers();
		p=g;
		g=g->next;
		delete p;
		if(!g) {k=0; asm cli; if(a) System::unlock(); return;}
	}
	p=g;
	while(p->next)
	{
		if(PCB::globalSignalState[p->next->sig] && PCB::running->localSignalState[p->next->sig])
		{
			if(p->next==k) k=p;
			//printf("\nobradjuje se signal %d nit %d\n",p->next->sig,PCB::getPRunningId());
			PCB::running->handlers[p->next->sig]->callHandlers();
			snode* q=p->next;
			p->next=p->next->next;
			delete q;
		}
		else p=p->next;
	}
	asm cli;
	if(a) System::unlock();
}
int SignalList::empty()
{
	if(g) return 0;
	return 1;
}
SignalList::~SignalList()
{
	while(g)
	{
		snode* p=g;
		g=g->next;
		delete p;
	}
	g=0;
	k=0;
}
