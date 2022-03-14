#ifndef IVT_H_
#define IVT_H_
#include "kerev.h"
typedef void interrupt (*pInterrupt)(...);
class KernelEv;
class IVTEntry
{
public:
	pInterrupt oldRoutine;
	static IVTEntry** allEntries;
	int number;
	KernelEv* owner;
	IVTEntry(int no,pInterrupt old);
	void callOld();
	static IVTEntry* getEntry(int no);
	void signal();
	static void dispose();
	~IVTEntry();
};




#endif /* IVT_H_ */
