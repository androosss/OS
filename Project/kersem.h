#ifndef KERSEM_H_
#define KERSEM_H_
#include "slist.h"
#include "pcb.h"
#include "sleep.h"
#include "system.h"
#include "kersem.h"
#include "SCHEDULE.H"

class slist;
class Sleep;
class KernelSem
{
	public:
		static slist* allSems;
		static int id;
		int value;
		int pid;
		Sleep* sleepList;
		KernelSem(int init);
		~KernelSem();
		int wait(Time time);
		int signal(int n);
		int val() const;
		void tack();
		static void dispose();
};

#endif /* KERSEM_H_ */
