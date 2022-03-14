#include "pcb.h"
#include "SCHEDULE.H"
#include <dos.h>
#include "user.h"
ID PCB::gid=0;
volatile PCB* PCB::running=0;
PCBlist* PCB::tlist=0;
Thread* PCB::idle=0;
Thread* PCB::maint=0;
int* PCB::globalSignalState=0;
PCB::PCB(StackSize s,Time t,Thread* th)
{
	if(!globalSignalState)
	{
		globalSignalState=new int[16];
		for(int i=0; i<16; i++)
			globalSignalState[i]=1;
	}
	localSignalState=new int[16];
	handlers=new HandlerList*[16];
	signals=new SignalList();
	if(gid==0)
	{
		parent=0;
		for(int i1=0; i1<16; i1++)
		{
			localSignalState[i1]=1;
			handlers[i1]=new HandlerList();
		}
	}
	else
	{
		parent=(PCB*)PCB::running;
		for(int i2=0; i2<16; i2++)
		{
			localSignalState[i2]=parent->localSignalState[i2];
			handlers[i2]=new HandlerList();
			handlers[i2]->copy(parent->handlers[i2]);
		}
	}
	pid=gid;
	gid++;
	state=0;
	SemFlag=0;
	if(stackSize==0) stackSize=4096;
	stackSize=s/sizeof(unsigned);
	if(stackSize> 65535) stackSize= 65535;
	timeSlice=t;
	myThread=th;
	stack=0;
	ss=0;
	sp=0;
	bp=0;
	waitList=0;
}
void PCB::run()
{
	myThread->run();
	while(!signals->empty())
	{
		dispatch();
	}
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
	signal(2);
	if(parent) parent->signal(1);
	state=4;
	//printf("gotova nit id %d \n",getPId());
	while (!waitList->empty())
	{
		PCB* p=waitList->remove();
		p->state=1;
		Scheduler::put(p);
		//printf("oslobodjena nit %d\n",p->getPId());
	}
	if(a) System::unlock();
}
void PCB::wrapper()
{
	((PCB*)running)->run();
	System::dispatch();
}
void PCB::startPCB()
{
	if(state!=0) return;
	waitList=new PCBlist();
	stack=new unsigned [stackSize];
	stack[stackSize-1]=0x200;
	#ifndef BCC_BLOCK_IGNORE
	stack[stackSize-2]=FP_SEG(wrapper);
    stack[stackSize-3]=FP_OFF(wrapper);
    sp = FP_OFF(stack+stackSize-12);
    ss = FP_SEG(stack+stackSize-12);
    bp = FP_OFF(stack+stackSize-12);
 	#endif
    state=1;
    if(!tlist) tlist=new PCBlist();
    tlist->put(this);
    //printf("pocinje nit %d \n",getPId());
    if(myThread!=idle) Scheduler::put(this);
}
ID PCB::getPId()
{
	return pid;
}
ID PCB::getPRunningId()
{
	return ((PCB*) running)->getPId();
}
PCB* PCB::getPCBById(ID id)
{
	return tlist->get(id);
}
void PCB::waitToComplete()
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
	if(state==0 || state==4 || this==running || myThread==idle)
	{
		if(a) System::unlock();
		return;
	}
	running->state=3;
	//printf("blokirana %d \n",getPRunningId());
	waitList->put((PCB*)running);
	System::unlock();
	System::dispatch();
	if(!a) System::lock();
}
PCB::~PCB()
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
	waitToComplete();
	System::lock();
	if(stack){
	delete waitList;
	delete [] stack;
	delete [] localSignalState;
	for(int i=0; i<16; i++)
		delete handlers[i];
	delete [] handlers;
	delete signals;}
	if(a) System::unlock();

}
void PCB::dtlist()
{
	delete tlist;
	delete [] globalSignalState;
}
void PCB::signal(SignalId signal)
{
	if(signal>15) return;
	signals->put(signal);
}
void PCB::registerHandler(SignalId signal, SignalHandler handler)
{
	if(signal>15) return;
	handlers[signal]->put(handler);
}
void PCB::unregisterAllHandlers(SignalId id)
{
	if(id>15) return;
	delete handlers[id];
	handlers[id]=new HandlerList();
}
void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2)
{
	if(id>15) return;
	handlers[id]->swap(hand1,hand2);
}
void PCB::blockSignal(SignalId signal)
{
	if(signal>15) return;
	localSignalState[signal]=0;
}
void PCB::blockSignalGlobally(SignalId signal)
{
	if(signal>15) return;
	globalSignalState[signal]=0;
}
void PCB::unblockSignal(SignalId signal)
{
	if(signal>15) return;
	localSignalState[signal]=1;
}
void PCB::unblockSignalGlobally(SignalId signal)
{
	if(signal>15) return;
	globalSignalState[signal]=1;
}
