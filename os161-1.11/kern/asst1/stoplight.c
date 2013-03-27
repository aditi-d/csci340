/* 
 * stoplight.c
 *
 * 31-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: You can use any synchronization primitives available to solve
 * the stoplight problem in this file.
 */


/*
 * 
 * Includes
 *
 */

#include <types.h>
#include <lib.h>
#include <test.h>
#include <thread.h>
#include <synch.h>
/*
 *
 * Constants
 *
 */

#define EAST 0
#define WEST 1
#define NORTH 2
#define SOUTH 3
#define RIGHT 0
#define LEFT 1
#define STRAIGHT 2
#define INTRNE 0
#define INTRNW 1
#define INTRSE 2
#define INTRSW 3


/*
 * Number of cars created.
 */

#define NCARS 20


/*
 *
 * Function Definitions
 *
 */

//Resources locks
struct lock* NE;
struct lock* NW;
struct lock* SE;
struct lock* SW;
int resource=-1;
struct semaphore *semstoplight;
/*
printcar()
*/
void printcar(int carnumber,int approachdirection,int turndirection,int intersection){
	const char *apprdir,*turndir,*intersect;
	switch(approachdirection){
		case EAST:
			apprdir="EAST";
			break;
		case WEST:
			apprdir="WEST";
			break;
		case NORTH:
			apprdir="NORTH";
			break;
		case SOUTH:
			apprdir="SOUTH";
			break;
	}
	switch(turndirection){
		case RIGHT:
			turndir="RIGHT";
			break;
		case LEFT:
			turndir="LEFT";
			break;
		case STRAIGHT:
			turndir="STRAIGHT";
			break;
		}
	switch(intersection){
		case INTRNW:
			intersect="NW";
			break;
		case INTRNE:
			intersect="NE";
			break;
		case INTRSW:
			intersect="SW";
			break;
		case INTRSE:
			intersect="SE";
			break;
		default:
			intersect="NONE";
			break;
	}

	kprintf("\nCar number %d approaching from %s (direction) turning %s (turndirection) crossing intersection %s\n",carnumber,apprdir,turndir,intersect);
}

/*
 * gostraight()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement passing straight through the
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
gostraight(unsigned long cardirection,
           unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */
        
        //(int) cardirection;
        //(int) carnumber;
	  switch(cardirection){
		case EAST:
			lock_acquire(NE);
			lock_acquire(NW);
			resource=0;
			printcar(carnumber,cardirection,STRAIGHT,resource);
			lock_release(NE);
			resource=1;
			printcar(carnumber,cardirection,STRAIGHT,resource);
			lock_release(NW);
			break;
		case WEST:
			lock_acquire(SW);
			lock_acquire(SE);
			resource=3;
			printcar(carnumber,cardirection,STRAIGHT,resource);
			lock_release(SW);
			resource=2;
			printcar(carnumber,cardirection,STRAIGHT,resource);
			lock_release(SE);
			break;

		case NORTH:
			lock_acquire(NW);
			lock_acquire(SW);
			resource=1;
			printcar(carnumber,cardirection,STRAIGHT,resource);
			lock_release(NW);
			resource=3;
			printcar(carnumber,cardirection,STRAIGHT,resource);
			lock_release(SW);
			break;
		case SOUTH:
			lock_acquire(SE);
			lock_acquire(NE);
			resource=2;
			printcar(carnumber,cardirection,STRAIGHT,resource);
			lock_release(SE);
			resource=0;
			printcar(carnumber,cardirection,STRAIGHT,resource);
			lock_release(NE);
			break;
	}	 
}

/*
 * turnleft()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a left turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnleft(unsigned long cardirection,
         unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */

        //(int) cardirection;
        //(int) carnumber;
	 	  switch(cardirection){
		case EAST:
			lock_acquire(NE);
			lock_acquire(NW);
			lock_acquire(SW);			
			resource=0;
			printcar(carnumber,cardirection,LEFT,resource);
			lock_release(NE);
			resource=1;
			printcar(carnumber,cardirection,LEFT,resource);
			lock_release(NW);
			resource=3;
			printcar(carnumber,cardirection,LEFT,resource);
			lock_release(SW);
			break;
		case WEST:
			lock_acquire(SW);
			lock_acquire(SE);
			lock_acquire(NE);
			resource=3;
			printcar(carnumber,cardirection,LEFT,resource);
			lock_release(SW);			
			resource=2;
			printcar(carnumber,cardirection,LEFT,resource);
			lock_release(SE);
			resource=0;
			printcar(carnumber,cardirection,LEFT,resource);
			lock_release(NE);
			break;
		case NORTH:
			lock_acquire(NW);
			lock_acquire(SW);
			lock_acquire(SE);
			resource=1;
			printcar(carnumber,cardirection,LEFT,resource);
			lock_release(NW);
			resource=3;
			printcar(carnumber,cardirection,LEFT,resource);			
			lock_release(SW);
			resource=2;
			printcar(carnumber,cardirection,LEFT,resource);
			lock_release(SE);
			break;
		case SOUTH:
			lock_acquire(SE);
			lock_acquire(NE);
			lock_acquire(NW);
			resource=2;
			printcar(carnumber,cardirection,LEFT,resource);
			lock_release(SE);
			resource=0;
			printcar(carnumber,cardirection,LEFT,resource);			
			lock_release(NE);
			resource=1;
			printcar(carnumber,cardirection,LEFT,resource);
			lock_release(NW);
			break;
	}	 

}


/*
 * turnright()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a right turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnright(unsigned long cardirection,
          unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */

        //(int) cardirection;
        //(int) carnumber;
	switch(cardirection){
		case EAST:
			lock_acquire(NE);
			resource=0;
			printcar(carnumber,cardirection,LEFT,resource);
			lock_release(NE);
			break;
		case WEST:
			lock_acquire(SW);
			resource=3;
			printcar(carnumber,cardirection,LEFT,resource);
			lock_release(SW);
			break;
		case NORTH:
			lock_acquire(NW);
			resource=1;
			printcar(carnumber,cardirection,LEFT,resource);
			lock_release(NW);
			break;
		case SOUTH:
			lock_acquire(SE);
			resource=2;
			printcar(carnumber,cardirection,LEFT,resource);
			lock_release(SE);
			break;
	}	 

}


/*
 * approachintersection()
 *
 * Arguments: 
 *      void * unusedpointer: currently unused.
 *      unsigned long carnumber: holds car id number.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Change this function as necessary to implement your solution. These
 *      threads are created by createcars().  Each one must choose a direction
 *      randomly, approach the intersection, choose a turn randomly, and then
 *      complete that turn.  The code to choose a direction randomly is
 *      provided, the rest is left to you to implement.  Making a turn
 *      or going straight should be done by calling one of the functions
 *      above.
 */
 
static
void
approachintersection(void * unusedpointer,
                     unsigned long carnumber)
{
        int cardirection;
	 int turndirection;
        /*
         * Avoid unused variable and function warnings.
         */

        (void) unusedpointer;
        //(void) carnumber;
//	(void) gostraight;
//	(void) turnleft;
//	(void) turnright;

        /*
         * cardirection is set randomly.
         */

        cardirection = random() % 4;
        turndirection=random()%3;
	printcar(carnumber,cardirection,turndirection,-1);
//	kprintf("\n----------Car num:: %d is comming from %d going to %d",(int)carnumber,cardirection,turndirection);
        //approachdir=random()%4;
		switch(turndirection){
			case RIGHT:
				turnright(cardirection,carnumber);
			break;
			case LEFT:
				turnleft(cardirection,carnumber);
			break;
			case STRAIGHT:
				gostraight(cardirection,carnumber);
			break;
		}
        V(semstoplight);
}

/*
 * createcars()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up the approachintersection() threads.  You are
 *      free to modiy this code as necessary for your solution.
 */

int
createcars(int nargs,
           char ** args)
{
        int index, error,i;

        /*
         * Avoid unused variable warnings.
         */

        (void) nargs;
        (void) args;

	 NE=lock_create("NE");
	 NW=lock_create("NW");
	 SE=lock_create("SE");
	 SW=lock_create("SW");
	 semstoplight=sem_create("semstoplight",0);

        /*
         * Start NCARS approachintersection() threads.
         */

        for (index = 0; index < NCARS; index++) {

                error = thread_fork("approachintersection thread",
                                    NULL,
                                    index,
                                    approachintersection,
                                    NULL
                                    );

                /*
                 * panic() on error.
                 */

                if (error) {
                        
                        panic("approachintersection: thread_fork failed: %s\n",
                              strerror(error)
                              );
                }
        }
	for(i=0;i<NCARS;i++){
	        P(semstoplight);
	}        
	return 0;
}
