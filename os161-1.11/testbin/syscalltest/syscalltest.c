/*
 * Program to test argument passing: it displays the argc and all
 * of argv, and then exits.
 *
 * Intended for the basic system calls assignment. This may help
 * debugging the argument handling of execv().
 */

#include <unistd.h>

int
main(int argc, char *argv[])
{
	//int num;
	/*const char *tmp;
	int i;

	printf("argc: %d\n", argc);

	for (i=0; i<=argc; i++) {
		tmp = argv[i];
		if (tmp==NULL) {
			tmp = "[NULL]";
		}
		printf("argv[%d]: %s\n", i, tmp);
	}*/
	_helloworld();
	reboot(0);
	return 0;
}