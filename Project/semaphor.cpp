#include "semaphor.h"
Semaphore::Semaphore (int init)
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
	myImpl=new KernelSem(init);
	if(a) System::unlock();
}
int Semaphore::wait (Time maxTimeToWait)
{
	return myImpl->wait(maxTimeToWait);
}
int Semaphore::signal(int n)
{
	return myImpl->signal(n);
}
int Semaphore::val () const
{
	int b=0;
	if(!System::lockFlag) {System::lock(); b=1;}
	int a=myImpl->val();
	if(b) System::unlock();
	return a;
}
Semaphore::~Semaphore()
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
	delete myImpl;
	if(a) System::unlock();
}
