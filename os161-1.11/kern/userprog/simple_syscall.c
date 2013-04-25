#include<types.h>
#include<lib.h>
#include<syscall.h>
#include<pid.h>
#include<thread.h>

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

int sys_getpid(int *retval){
	pid_t pid;
	pid=getPid();
	*retval=(int)pid;
	//kprintf("\n in sys_getpid fun::%d",pid);
	/*if(syscall is successfull){
		return 0;
	}
	return -1; */
	return pid;
}

int sys_printchar(char c){
	//err_retval is 0 for now
	int err_retval=0;
	putch(c);
	return err_retval;
}

int sys_readchar(char *ch){
	*ch=getch();
	return 0;
}


