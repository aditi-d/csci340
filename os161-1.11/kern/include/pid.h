#include<types.h>
#include<synch.h>
#include<thread.h>
#define MAX_PROCS 65535

typedef struct{
	pid_t pid;
	struct thread *threadPtr;
}pidNode;

pidNode processIdList[MAX_PROCS];
 
pid_t getNextPid();
void assignPid();
pid_t getPid();
