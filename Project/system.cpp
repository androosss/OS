#include "system.h"

#include "SCHEDULE.H"
volatile unsigned System::lockFlag=0;
volatile unsigned System::dispatchFlag=0;
volatile unsigned System::context_switch_request=0;
volatile unsigned System::counter=1;
PCB* System::target=0;
pInterrupt System::oldIntr=0;
unsigned oldTimerOFF, oldTimerSEG;
unsigned tbp=0;
unsigned tsp=0;
unsigned tss=0;
PCB* kerpcb;
extern void tick();
void interrupt timer(...)
{
	if (System::dispatchFlag==0 && System::counter>0)
	{
		System::counter--;
	}
	if (System::dispatchFlag==0)
	{
		KernelSem::allSems->notify();
		System::oldIntr();
		tick();
	}
	System::dispatchFlag=0;
	if ((System::counter==0 && PCB::running->timeSlice!=0) || System::context_switch_request)
	{
		if(!System::lockFlag)
		{
			System::context_switch_request=0;
			#ifndef BCC_BLOCK_IGNORE
			asm{
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
			#endif
			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;
			/*System::lockFlag=1;
			printf("Promena konteksta id1: %d, ",PCB::getPRunningId());
			asm cli;
			System::lockFlag=0;*/
			PCB::running->signals->SignalAll();
			if (PCB::running->state==2)
			{
				PCB::running->state=1;
				if(PCB::running!=PCB::idle->myPCB)
					Scheduler::put((PCB *) PCB::running);
			}
			PCB* temp=Scheduler::get();
			if(temp==0)
			{
				PCB::running=PCB::idle->myPCB;
				PCB::idle->myPCB->state=2;
			}
			else
			{
				PCB::running=temp;
				PCB::running->state=2;
			}
			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;

			System::counter=PCB::running->timeSlice;

			#ifndef BCC_BLOCK_IGNORE
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
			#endif

			if(System::target)
			{
				System::lock();
				delete [] System::target->stack;
				System::target->stack=0;
				delete System::target->localSignalState;
				System::target->localSignalState=0;
				delete System::target->signals;
				System::target->signals=0;
				delete System::target->waitList;
				System::target->waitList=0;
				for(int i=0; i<16; i++)
				{
					delete System::target->handlers[i];
					System::target->handlers[i]=0;
				}
				delete [] System::target->handlers;
				System::target->handlers=0;
				System::target->state=4;
				System::target=0;
				System::unlock();
			}
		}
		else System::context_switch_request=1;
	}
}

void System::initialize(int argc,char** argv)
{
		System::lock();
		#ifndef BCC_BLOCK_IGNORE
		oldIntr=getvect(0x08);
		setvect(0x08,timer);
		#endif
		kerpcb=new PCB(0,0,0);
		PCB::running=kerpcb;
		kerpcb->registerHandler(0,killThread);
		PCB::idle=new IdleThread();
		PCB::idle->myPCB->timeSlice=1;
		PCB::maint=new MainThread(argc,argv);
		PCB::idle->start();
		PCB::maint->start();
		System::unlock();
}
void System::restore()
{
		System::lock();
		#ifndef BCC_BLOCK_IGNORE
		setvect(0x08,oldIntr);
		#endif
		delete PCB::maint;
		delete PCB::idle;
		delete kerpcb;
		PCB::dtlist();
		KernelSem::dispose();
		IVTEntry::dispose();
		System::unlock();
}
void System::dispatch(){
		asm cli;
		context_switch_request=1;
		dispatchFlag=1;
		timer();
		asm sti;
}
void killThread()
{
	System::target=(PCB*)PCB::running;
	if(System::target->getPId()<3) return;
	System::target->state=3;
	/* if(System::target->parent) System::target->parent->signal(1);
	 * System::target->signal(2);
	 */
	while (!System::target->waitList->empty())
	{
		PCB* p=System::target->waitList->remove();
		p->state=1;
		Scheduler::put(p);
	}
}

