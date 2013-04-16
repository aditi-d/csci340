#ifndef _SYSCALL_H_
#define _SYSCALL_H_

/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */

int sys_reboot(int code);
int sys_helloworld(int code);
int sys_printint(int code,int num);
pid_t sys_getpid(int code);
int printchar(char);

#endif /* _SYSCALL_H_ */
