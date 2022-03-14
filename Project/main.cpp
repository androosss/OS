#include "thread.h"
#include "system.h"
#include "idle.h"
#include "pcb.h"
#include "mainthr.h"
#include "user.h"


int main(int argc, char* argv[])
{
	//printf("start\n");
	System::initialize(argc,argv);
	//printf("cekam main \n");
	PCB::maint->waitToComplete();
	//printf("kraj main \n");
	int ret=MainThread::sol;
	System::restore();
	//printf("end result=%d\n",ret);
	return ret;
}
