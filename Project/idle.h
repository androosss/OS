#ifndef IDLE_H_
#define IDLE_H_

class IdleThread:public Thread
{
	public:
		void run()
		{
			for(long i=0; i<100000000; i++)
			{
				if(i==99999999)
				{
					//printf("idle \n");
					i=0;
				}
			}
		}
};



#endif /* IDLE_H_ */
