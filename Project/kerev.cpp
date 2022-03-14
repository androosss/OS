#include "kerev.h"
KernelEv* KernelEv::current=0;
KernelEv::KernelEv(IVTNo ivtNo)
{
	myPCB=((PCB*)PCB::running);
	val=1;
	current=this;
	if(IVTEntry::getEntry(ivtNo)) myEntry=IVTEntry::getEntry(ivtNo);
}
KernelEv::~KernelEv()
{
	int a=0;
	if(!System::lockFlag) {a=1; System::lock();}
	if(a) System::unlock();
}
void KernelEv::signal()
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
	System::lock();
	//printf("signal\n");
	if(val==1) {if(a) System::unlock(); return;}
	if(val==0) {val=1; if(a) System::unlock(); return;}
	myPCB->state=1;
	val=0;
	Scheduler::put(myPCB);
	if(a) System::unlock();
}
void KernelEv::wait()
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
	//printf("val=%d\n",val);
	if (PCB::running!=myPCB || val==-1) {if(a) System::unlock(); return;}
	//printf("wait2\n");
	val--;
	if(val==-1)
	{
		myPCB->state=3;
		System::unlock();
		System::dispatch();
		if(!a) System::lock();
	}
	else if(a) System::unlock();
}

