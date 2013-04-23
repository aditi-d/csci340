#ifndef _SYSCALL_H_
#define _SYSCALL_H_
#include<thread.h>

/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */

int sys_reboot(int code);
int sys_helloworld(int code);
int sys_printint(int code,int num);
int sys_getpid(int* retval);
int sys_printchar(char ch);
int sys_fork(struct trapframe*,struct addrspace*);
int sys_readchar(char* ch);
#endif /* _SYSCALL_H_ */
