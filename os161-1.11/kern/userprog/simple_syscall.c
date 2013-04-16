#include<types.h>
#include<lib.h>
#include<syscall.h>
#include<pid.h>

int sys_helloworld(int code){
	int retval;
	retval=kprintf("HELLO WORLD\n");
	return retval;
}

int sys_printint(int code,int num){
	int retval;
	retval=kprintf("%d",num);
	return retval;
}

pid_t sys_getpid(int code){
	pid_t pid;
	pid=getPid();
	return pid;
}

int sys_printchar(char c){
	int retval=0;
	kprintf("\n printing::");
	putch(c);
	return retval;
}