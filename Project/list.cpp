#include "list.h"
PCBlist::PCBlist()
{
	g=0;
	k=0;
}
PCB* PCBlist::get(ID a)
{
        node* p=g;
        while(p!=0)
        {
            if((p->val)->getPId()==a)
                return p->val;
            p=p->next;
        }
        return 0;
    }
void PCBlist::put(PCB* a)
    {
        if(g==0)
        {
            node* p=new node(a);
            g=k=p;
            return;
        }
        if(g!=k)
        {
            node *p =new node(a);
            k->next=p;
            k=p;
            return;
        }
        node *p =new node(a);
        k->next=p;
        g->next=p;
        k=p;
        return;
}
PCB* PCBlist::remove()
    {
    	if(!g) return 0;
    	node* p=g;
    	g=g->next;
    	if(!g) k=0;
    	return p->val;
    }
int PCBlist::empty()
    {
    	if(g) return 0;
    	else return 1;
    }
PCBlist::~PCBlist()
{
    while(g!=0)
    {
        node* p=g;
        g=g->next;
        delete p;
    }
    if (g) delete g;
    if (k) delete k;
}
