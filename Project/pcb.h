#ifndef PCB_H_
#define PCB_H_
#include "thread.h"
#include "system.h"
#include "list.h"
#include "hlist.h"
#include <dos.h>

class SignalList;
class HandlerList;
class PCBlist;
class PCB
{
	public:
		static volatile PCB* running;
		static Thread* idle;
		static PCBlist* tlist;
		static Thread* maint;
		static int* globalSignalState;
		HandlerList** handlers;
		SignalList* signals;
		int* localSignalState;
		PCB* parent;
		unsigned* stack;
		int state; //0 Kreirana, 1 Spremna, 2 Izvrsava se, 3 Blokirana, 4 Zavrsena
		unsigned sp;
		unsigned ss;
		unsigned bp;
		int SemFlag;
		Thread* myThread;
		StackSize stackSize;
		Time timeSlice;
		PCBlist* waitList;
		ID getPId();
		static ID getPRunningId();
		static PCB* getPCBById(ID id);
		PCB(StackSize s,Time t,Thread* th);
		void startPCB();
		void run();
		static void wrapper();
		static void dtlist();
		void signal(SignalId signal);
		void registerHandler(SignalId signal, SignalHandler handler);
		void unregisterAllHandlers(SignalId id);
		void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
		void blockSignal(SignalId signal);
		static void blockSignalGlobally(SignalId signal);
		void unblockSignal(SignalId signal);
		static void unblockSignalGlobally(SignalId signal);
		void waitToComplete();
		~PCB();
	private:
		static ID gid;
		ID pid;

};

#endif /* PCB_H_ */
