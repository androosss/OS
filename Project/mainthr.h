#ifndef MAINTHREAD_H_
#define MAINTHREAD_H_
#include "thread.h"
#include "stdio.h"
class MainThread:public Thread
{
public:
	static int sol;
	int arg1;
	char** arg2;
	MainThread(int argc, char* argv[]);
	void run();
};



#endif /* MAINTHREAD_H_ */
