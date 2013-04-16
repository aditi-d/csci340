#include<types.h>
#include<thread.h>
#include<pid.h>
#include<curthread.h>

pid_t getNextPid(){
	int i;
	for(i=0;;i++){
		if(processIdList[i].threadPtr==NULL){
			break;
		}
	}	
	return i;
}

void assignPid(){
	struct lock *pidlock;
	pid_t pid;
	pidNode pidnode;
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


