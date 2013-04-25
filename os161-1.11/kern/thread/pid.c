#include<types.h>
#include<thread.h>
#include<pid.h>
#include<curthread.h>
#include<lib.h>
static struct pidNode *processIdList;

pid_t getNextPid(){
	int i;
	for(i=0;;i++){
		if(processIdList[i].threadPtr==NULL){
			break;
		}
	}	
	//kprintf("\n getNextPid:: pid is::%d",i);
	return i;
}

void assignPid(){
	struct lock *pidlock;
	pid_t pid;
	struct pidNode pidnode;
	pidlock=lock_create("pidlock");
	lock_acquire(pidlock);
	pid=getNextPid();
	pidnode.pid=pid;
	pidnode.threadPtr=curthread;
	curthread->pid=pid;
	processIdList[pid]=pidnode;
	lock_release(pidlock);
}

pid_t getPid(){
	return curthread->pid;
}

void createPidTable(){
	//kprintf("\nin pid.c: createPidTable:");
	processIdList=kmalloc(sizeof(struct pidNode)*65536);//(kmalloc(sizeof((struct pidNode)*65535)));
	//return pidNode;
}

struct pidNode* getPidTable(){
	//kprintf("\npidlib::%x::",processIdList);
	return processIdList;
}

