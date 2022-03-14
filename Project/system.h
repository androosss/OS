#ifndef SYSTEM_H_
#define SYSTEM_H_
#include "pcb.h"
#include "stdio.h"
#include "stdlib.h"
#include "idle.h"
#include "mainthr.h"
#include "kersem.h"
#include "ivt.h"

typedef void interrupt (*pInterrupt)(...);
class System
{
public:
	static volatile unsigned lockFlag;
	static volatile unsigned dispatchFlag;
	static volatile unsigned context_switch_request;
	static volatile unsigned counter;
	static PCB* target;
	static pInterrupt oldIntr;
	static void initialize(int argc,char** argv);
	static void restore();
	static void lock()
	{
		lockFlag=1;
	}
	static void unlock()
	{
		lockFlag=0;
	}
	static void dispatch();
};
void killThread();

 #endif /* SYSTEM_H_ */
