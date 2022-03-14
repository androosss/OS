#include "thread.h"
#include "pcb.h"

ID Thread::getRunningId()
{
	return PCB::getPRunningId();
}
ID Thread::getId()
{
	return this->myPCB->getPId();
}
Thread* Thread::getThreadById(ID id)
{
	return PCB::getPCBById(id)->myThread;
}
Thread::Thread(StackSize stackSize, Time timeSlice)
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
	this->myPCB=new PCB(stackSize,timeSlice,this);
	if(a) System::unlock();
}
void Thread::waitToComplete()
{
	this->myPCB->waitToComplete();
}
Thread::~Thread()
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
	delete this->myPCB;
	if(a) System::unlock();
}
void dispatch()
{
	System::dispatch();
}
void Thread::start()
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
	myPCB->startPCB();
	if(a) System::unlock();
}
void Thread::signal(SignalId signal)
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
	myPCB->signal(signal);
	if(a) System::unlock();
}
void Thread::registerHandler(SignalId signal, SignalHandler handler)
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
	myPCB->registerHandler(signal,handler);
	if(a) System::unlock();
}
void Thread::unregisterAllHandlers(SignalId id)
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
	myPCB->unregisterAllHandlers(id);
	if(a) System::unlock();
}
void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2)
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
	myPCB->swap(id,hand1,hand2);
	if(a) System::unlock();
}
void Thread::blockSignal(SignalId signal)
{
	myPCB->blockSignal(signal);
}
void Thread::blockSignalGlobally(SignalId signal)
{
	PCB::blockSignalGlobally(signal);
}
void Thread::unblockSignal(SignalId signal)
{
	myPCB->unblockSignal(signal);
}
void Thread::unblockSignalGlobally(SignalId signal)
{
	PCB::unblockSignalGlobally(signal);
}
