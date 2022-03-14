// File: event.h
#ifndef _event_h_
#define _event_h_
#include "kerev.h"
typedef unsigned char IVTNo;
class KernelEv;
class Event {
public:
 Event (IVTNo ivtNo);
 ~Event ();
 void wait ();
protected:
 friend class KernelEv;
 void signal(); // can call KernelEv
private:
 KernelEv* myImpl;
};


#define PREPAREENTRY(ivtNo,flag)\
void interrupt inter##ivtNo(...);\
IVTEntry entry##ivtNo(ivtNo, inter##ivtNo);\
void interrupt inter##ivtNo(...) {\
	entry##ivtNo.signal();\
	if(##flag)\
	entry##ivtNo.callOld();\
}
#endif
