#include<types.h>
#include<lib.h>
#include<syscall.h>

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
