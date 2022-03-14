#include "kersem.h"
slist* KernelSem::allSems=0;
int KernelSem::id=0;
KernelSem::KernelSem(int init)
{
	pid=id;
	id++;
	if(!allSems)
	{
		allSems=new slist();
	}
	value=init;
	sleepList=new Sleep(this);
	allSems->put(this);
}
KernelSem::~KernelSem()
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
	delete sleepList;
	if(a) System::unlock();
}
int KernelSem::wait(Time time)
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
	value--;
	if(value<0)
	{
		PCB::running->state=3;
		sleepList->put(((PCB*)PCB::running),time,0);
		System::unlock();
		System::dispatch();
		if(!a) System::lock();
	}
	else if(a) System::unlock();
	return PCB::running->SemFlag;
}
int KernelSem::signal(int n)
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
	if(n==0)
	{
		value++;
		if(value<1)
		{
			PCB* temp=sleepList->remove();
			temp->SemFlag=1;
			if(temp->state!=4)
			{
				temp->state=1;
				Scheduler::put(temp);
			}
		}
		if(a) System::unlock();
		return 0;
	}
	if(n>0)
	{
		int released=0;
		while(value<1 && n>0)
		{
			value++;
			n--;
			PCB* temp=sleepList->remove();
			if(temp){
			temp->SemFlag=1;
			if(temp->state!=4)
			{
				temp->state=1;
				Scheduler::put(temp);
			}
			released++;}
		}
		value+=n;
		if(a) System::unlock();
		return released;
	}
	if(a) System::unlock();
	return n;
}
int KernelSem::val() const
{
	return value;
}
void KernelSem::tack()
{
	sleepList->tack();
}
void KernelSem::dispose()
{
	delete allSems;
}
