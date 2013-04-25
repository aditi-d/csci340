#include<types.h>
#include<synch.h>
#include<thread.h>
#define MAX_PROCS 65535

struct pidNode{
	pid_t pid;
	struct thread *threadPtr;
};
 
pid_t getNextPid();
void assignPid();
pid_t getPid();
void createPidTable();
struct pidNode* getPidTable();
