#include "mainthr.h"
int MainThread::sol=0;
extern int  userMain(int argc, char* argv[]);
MainThread::MainThread(int argc, char* argv[])
{
	arg1=argc;
	arg2=argv;
}
void MainThread::run()
{
	sol=userMain(arg1,arg2);
}

