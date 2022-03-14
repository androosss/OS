#ifndef HLIST_H_
#define HLIST_H_
#include "pcb.h"
#include "system.h"
typedef void (*SignalHandler)();
typedef unsigned SignalId;
class HandlerList
{
public:
	struct hnode{
		hnode* next;
		SignalHandler hand;
		hnode(SignalHandler h)
		{
			next=0;
			hand=h;
		}
	};
	hnode* g;
	hnode* k;
	HandlerList();
	void put(SignalHandler h);
	void swap(SignalHandler h1,SignalHandler h2);
	void copy(HandlerList* list);
	void callHandlers();
	~HandlerList();
};

class SignalList
{
public:
	struct snode{
		snode* next;
		int sig;
		snode(int s)
		{
			next=0;
			sig=s;
		}
	};
	snode* g;
	snode* k;
	SignalList();
	void put(int i);
	void SignalAll();
	int empty();
	~SignalList();
};


#endif /* HLIST_H_ */
