#ifndef KEREV_H_
#define KEREV_H_
#include "event.h"
#include "pcb.h"
#include "ivt.h"
typedef void interrupt (*pInterrupt)(...);
typedef unsigned char IVTNo;
class IVTEntry;
class KernelEv
{
	public:
		static KernelEv* current;
		PCB* myPCB;
		IVTEntry* myEntry;
		int val;
		KernelEv(IVTNo ivtNo);
		~KernelEv();
		void signal();
		void wait();
};


#endif /* KEREV_H_ */
