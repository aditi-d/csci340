/*
 * Program to test argument passing: it displays the argc and all
 * of argv, and then exits.
 *
 * Intended for the basic system calls assignment. This may help
 * debugging the argument handling of execv().
 */

#include<stdio.h> 
#include<unistd.h>

int
main(int argc, char *argv[])
{
	int pid;
	char ch='c',ch2;
	pid=getpid();
	pid=_printchar(ch);
	ch2=_readchar();
	pid=_printchar(ch2);
	return 0;
}
