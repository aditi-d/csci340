#include<types.h>
#include<lib.h>
#include<syscall.h>
#include<pid.h>
#include<thread.h>
#include <addrspace.h>
#include <vm.h>
#include <vfs.h>
#include <kern/errno.h>
#include <kern/unistd.h>
#define NAME_MAX  255
#define PATH_MAX   1024

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

int sys_execv(const char *prog, char *const *args){
	char *tempdest,**dest;
	const char *userdata;
	int i,j=0,count=0,padding_start=0,next=0,real_counter=0,retval,len;
	struct vnode *v;
	//dest=kmalloc(sizeof(char*)*strlen(args));
	len=sizeof(args)/sizeof(char*); //no of elements in args
	dest=kmalloc(len*sizeof(char*)); 
	vaddr_t entrypoint, stackptr;
	int result;
	//=kmalloc(len);
	//int len=sizeof(prog)+sizeof(args);
	userdata=prog;

	kprintf("\n in execv1");

	//copyin pointers in kernel buffer and align by 4
	kprintf("\n in execv2");
	do{
		kprintf("\n in execv2");
		size_t len=strlen(userdata);//
		tempdest=kmalloc(sizeof(char)*len);	
		retval=copyin((const_userptr_t)userdata,tempdest,len);
		if(retval){
			kprintf("\nreturning");
			return retval;
		}
		kprintf("\n %s %d %d",userdata,retval,len);
		for(count=0;tempdest[count]!='\0';count++);
		padding_start=count;
		//count-1 should be a multiple of 4 else find the next multiple
		count=((count-1)%4==0)?count-1:count+(count-1)%4;
		for(i=padding_start;i<count;i++){
			tempdest[i]='\0';
		}
		kprintf("\ncrossed");
		dest[j]=kmalloc(sizeof(char)*strlen(tempdest));
		dest[j]=tempdest;
		kprintf("\ncrossed");
		userdata=NULL;
		userdata=args[next];
		next++;j++;real_counter=j;
	}while(userdata!=NULL);

	kprintf("\ncrossed");

	//open the executable, create new addr space and load elf into it
	kprintf("\n in execv3");
	/* Open the file. */; 
	result = vfs_open(prog, O_RDONLY, &v);
	kprintf("\n vfs_open result::%d",result);
	if (result) {
		return result;
	}
	kprintf("\ncrossed");

	/* Create a new address space. */
	curthread->t_vmspace = as_create();
	if (curthread->t_vmspace==NULL) {
		vfs_close(v);
		return ENOMEM;
	}
	
	kprintf("\ncrossed");

	/* Activate it. */
	as_activate(curthread->t_vmspace);

	/* Load the executable. */
	result = load_elf(v, &entrypoint);
	if (result) {
		/* thread_exit destroys curthread->t_vmspace */
		vfs_close(v);
		return result;
	}

	kprintf("\ncrossed");

	/* Done with the file now. */
	vfs_close(v);

	/* Define the user stack in the address space */
	result = as_define_stack(curthread->t_vmspace, &stackptr);
	if (result) {
		/* thread_exit destroys curthread->t_vmspace */
		return result;
	}

	kprintf("\ncrossed");

	//copy args into user stack
	kprintf("\n in execv4");
	
	/*for(i=0;i<len;i++){
		int len_each_str=strlen(dest[i]);
		char *temp=dest[i];
		for(j=0;j<len_each_str;j++){
			//stackptr[i]=*dest;
			stackptr[i]=temp[j];
		}
	}*/

	copyout(dest,stackptr,real_counter);

	//return to user mode using enter_new_process
	kprintf("\n in execv5,%d",len);
	/* Warp to user mode. */
	md_usermode(len /*argc*/, stackptr /*userspace addr of argv*/,
		    stackptr, entrypoint);
	
	/* md_usermode does not return */
	panic("md_usermode returned\n");
	return EINVAL;	
}