#include<types.h>
#include<lib.h>
#include<syscall.h>

int sys_helloworld(int code){
	int retval;
	retval=kprintf("HELLO WORLD\n");
	return retval;
}
