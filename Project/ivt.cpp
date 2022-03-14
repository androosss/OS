#include "ivt.h"
IVTEntry** IVTEntry::allEntries=0;
IVTEntry::IVTEntry(int no,pInterrupt newRoutine)
{
	int a=0;
	if(!System::lockFlag) {System::lock(); a=1;}
	if(!allEntries)
	{
		allEntries=new IVTEntry*[256];
		for(int i=0; i<256; i++)
			allEntries[i]=0;
	}
	owner=0;
	number=no;
	oldRoutine=0;
	#ifndef BCC_BLOCK_IGNORE
	oldRoutine=getvect(no);
	setvect(no,newRoutine);
	#endif
	if(allEntries[no]!=0) delete allEntries[no];
	allEntries[no]=this;
	if(a) System::unlock();
}
void IVTEntry::callOld()
{
	if(number>0) oldRoutine();
}
void IVTEntry::signal()
{
	if(number>0) owner->signal();
}
IVTEntry::~IVTEntry()
{
	#ifndef BCC_BLOCK_IGNORE
	if(number<256 && number>=0)
	{
		setvect(number,oldRoutine);
		//callOld();
	}
	#endif
}
IVTEntry* IVTEntry::getEntry(int no)
{
	int b=0;
	if(!System::lockFlag) {System::lock(); b=1;}
	if(no<0 || no>255 || allEntries[no]==0) {if (b) System::unlock(); return 0;}
	IVTEntry* a=allEntries[no];
	a->owner=KernelEv::current;
	if(b) System::unlock();
	return a;
}
void IVTEntry::dispose()
{
	for(int i=0; i<256; i++)
	{
		delete allEntries[i];
	}
	delete [] allEntries;
}
