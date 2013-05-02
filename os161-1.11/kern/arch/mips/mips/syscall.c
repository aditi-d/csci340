#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <machine/pcb.h>
#include <machine/spl.h>
#include <machine/trapframe.h>
#include <kern/callno.h>
#include <syscall.h>
#include<thread.h>
#include<curthread.h>
#include<addrspace.h>
#include<pid.h>

/*
 * System call handler.
 *
 * A pointer to the trapframe created during exception entry (in
 * exception.S) is passed in.
 *
 * The calling conventions for syscalls are as follows: Like ordinary
 * function calls, the first 4 32-bit arguments are passed in the 4
 * argument registers a0-a3. In addition, the system call number is
 * passed in the v0 register.
 *
 * On successful return, the return value is passed back in the v0
 * register, like an ordinary function call, and the a3 register is
 * also set to 0 to indicate success.
 *
 * On an error return, the error code is passed back in the v0
 * register, and the a3 register is set to 1 to indicate failure.
 * (Userlevel code takes care of storing the error code in errno and
 * returning the value -1 from the actual userlevel syscall function.
 * See src/lib/libc/syscalls.S and related files.)
 *
 * Upon syscall return the program counter stored in the trapframe
 * must be incremented by one instruction; otherwise the exception
 * return code will restart the "syscall" instruction and the system
 * call will repeat forever.
 *
 * Since none of the OS/161 system calls have more than 4 arguments,
 * there should be no need to fetch additional arguments from the
 * user-level stack.
 *
 * Watch out: if you make system calls that have 64-bit quantities as
 * arguments, they will get passed in pairs of registers, and not
 * necessarily in the way you expect. We recommend you don't do it.
 * (In fact, we recommend you don't use 64-bit quantities at all. See
 * arch/mips/include/types.h.)
 */

void
mips_syscall(struct trapframe *tf)
{
	int callno;
	int32_t retval;
	int err;

	assert(curspl==0);

	/*
	 * Initialize retval to 0. Many of the system calls don't
	 * really return a value, just 0 for success and -1 on
	 * error. Since retval is the value returned on success,
	 * initialize it to 0 by default; thus it's not necessary to
	 * deal with it except for calls that return other values, 
	 * like write.
	 */

	retval = 0;
	callno=tf->tf_v0;
    char ch;
	switch (callno) {
	    case SYS_reboot:
		err = sys_reboot(tf->tf_a0);
		break;

        case SYS__exit:
         thread_exit();
        break;
	    /* Add stuff here */
	    /*calls for assignment 0*/
	    case SYS__helloworld:
		//err = sys_helloworld(tf->tf_a0);
		break;
 
	    case SYS__printint:
		//err=sys_printint(tf->tf_a0,tf->tf_a1);
		break;

	    case SYS_getpid:
		err=sys_getpid(&retval);		
		break;

	    case SYS__printchar:
		err=sys_printchar(tf->tf_a0);
		break;

	    case SYS__readchar:
		err=sys_readchar(&ch);
		//kprintf("\nin syscall::%c",ch);
		break;	

	    case SYS_fork:
		//kprintf("\n ::current addr space::%x",curthread->t_vmspace);
		err=sys_fork(tf,curthread->t_vmspace);
		break;
		
	    case SYS_execv:
		err=sys_execv(tf->tf_a0,tf->tf_a1);
		//(const char *prog, char *const *args);
		//prog points to a const string, const char pointer pointing to a const string
		break;

	    default:
		kprintf("Unknown syscall %d\n", callno);
		//kprintf("\ncallno::%d",tf->tf_v0);
		err = ENOSYS;
		break;
	}


	if (err) {
		/*
		 * Return the error code. This gets converted at
		 * userlevel to a return value of -1 and the error
		 * code in errno.
		 */
		tf->tf_v0 = err;
		tf->tf_a3 = 1;      /* signal an error */
	}
	else {
		/* Success. */
		if(callno==SYS__readchar){
			tf->tf_v0=ch;
			tf->tf_a0=0;
		}
		else{
			tf->tf_v0 = retval;
			tf->tf_a3 = 0;      /* signal no error */
		}
	}
	
	/*
	 * Now, advance the program counter, to avoid restarting
	 * the syscall over and over again.
	 */
	
	tf->tf_epc += 4;

	/* Make sure the syscall code didn't forget to lower spl */
	assert(curspl==0);
    //kprintf("Exiting mips_syscall\n");
}

void md_forkentry(struct trapframe *tf,unsigned long addrspace_copy)
{
	/*
	 * This function is provided as a reminder. You need to write
	 * both it and the code that calls it.
	 *
	 * Thus, you can trash it and do things another way if you prefer.
	 */
	//(void)tf;
	//kprintf("\n md_forkentry");
	tf->tf_v0=0;
	tf->tf_a3=0;
	curthread->t_vmspace=(struct addrspace*)addrspace_copy;
	if(curthread->t_vmspace!=NULL){
		as_activate(curthread->t_vmspace);
	}

	//copy modified trap frame from kernel heap to stack
	struct trapframe tf_temp=*tf;//=kmalloc(sizeof(struct trapframe));
	tf_temp.tf_v0=tf->tf_v0;
	tf_temp.tf_a0=tf->tf_a0;
	tf_temp.tf_a1=tf->tf_a1;
	tf_temp.tf_a2=tf->tf_a2;
	tf_temp.tf_a3=tf->tf_a3; 
	tf->tf_epc+=4;
	mips_usermode(&tf_temp);
}

int sys_fork(struct trapframe *tf,struct addrspace *parent_addr_space){
	
	//kprintf("\n in sys_fork");
	struct thread *child_thread=NULL;
	struct thread *test;
	struct addrspace *parent_addr_space_copy;

	//create a new address space
	//kprintf("\n in sys_fork:creating new addr space");
	parent_addr_space_copy=as_create();
	if(parent_addr_space_copy==NULL)
		return ENOMEM;

	//make a copy of the parent's address space
	//kprintf("\n in sys_fork:copying to the addr space %x",parent_addr_space);
	as_copy(parent_addr_space,&parent_addr_space_copy);
	//kprintf("\n in sys_fork:copying to the addr space %x",*(parent_addr_space_copy));

	//make a copy of the parent`s trapframe on kernel heap
	//kprintf("\n in sys_fork:copying trap frame");
	struct trapframe *tf_temp=kmalloc(sizeof(struct trapframe));
	if(tf_temp==NULL){
		kfree(tf_temp);
		return ENOMEM;
	}
	memcpy(tf_temp,tf,sizeof(struct trapframe));

	//call thread_fork
	//pass the trapframe and the address space of the parent to the child`s thread_fork
	//kprintf("\n in sys_fork:forking");
	thread_fork("child thread",tf_temp,(unsigned long)parent_addr_space_copy,md_forkentry,&child_thread);

	//kprintf("\nchild_thread::%x",*(child_thread));
	//copy other required stuff and return with child`s pid
	//test=(child_thread);
	return child_thread->pid;
}

