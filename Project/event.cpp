#include "event.h"
Event::Event (IVTNo ivtNo)
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
	myImpl=new KernelEv(ivtNo);
	if(a) System::unlock();
}
Event::~Event ()
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
	delete myImpl;
	if(a) System::unlock();
}
void Event::wait ()
{
	myImpl->wait();
}
void Event::signal()
{
	myImpl->signal();
}

