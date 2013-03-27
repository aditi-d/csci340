/*
 * Synchronization primitives.
 * See synch.h for specifications of the functions.
 */

#include <types.h>
#include <lib.h>
#include <synch.h>
#include <thread.h>
#include <curthread.h>
#include <machine/spl.h>

////////////////////////////////////////////////////////////
//
// Semaphore.

struct semaphore *
sem_create(const char *namearg, int initial_count)
{
	struct semaphore *sem;

	assert(initial_count >= 0);

	sem = kmalloc(sizeof(struct semaphore));
	if (sem == NULL) {
		return NULL;
	}

	sem->name = kstrdup(namearg);
	if (sem->name == NULL) {
		kfree(sem);
		return NULL;
	}

	sem->count = initial_count;
	return sem;
}

void
sem_destroy(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);

	spl = splhigh();
	assert(thread_hassleepers(sem)==0);
	splx(spl);

	/*
	 * Note: while someone could theoretically start sleeping on
	 * the semaphore after the above test but before we free it,
	 * if they're going to do that, they can just as easily wait
	 * a bit and start sleeping on the semaphore after it's been
	 * freed. Consequently, there's not a whole lot of point in 
	 * including the kfrees in the splhigh block, so we don't.
	 */

	kfree(sem->name);
	kfree(sem);
}

void 
P(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);

	/*
	 * May not block in an interrupt handler.
	 *
	 * For robustness, always check, even if we can actually
	 * complete the P without blocking.
	 */
	assert(in_interrupt==0);

	spl = splhigh();
	while (sem->count==0) {
		thread_sleep(sem);
	}
	assert(sem->count>0);
	sem->count--;
	splx(spl);
}

void
V(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);
	spl = splhigh();
	sem->count++;
	assert(sem->count>0);
	thread_wakeup(sem);
	splx(spl);
}

////////////////////////////////////////////////////////////
//
// Lock.

struct lock *
lock_create(const char *name)
{
	struct lock *lock;

	lock = kmalloc(sizeof(struct lock));
	if (lock == NULL) {
		return NULL;
	}
	lock->name = kstrdup(name);
	if (lock->name == NULL) {
		kfree(lock);
		return NULL;
	}
	
	// add stuff here as needed
	lock->lockcurthread=NULL;

	return lock;
}

void
lock_destroy(struct lock *lock)
{
	int spl;
    assert(lock != NULL);
    
    //disable all interrupts
    spl=splhigh();
    
    //check if no threads are waiting on this lock
    //assert(thread_hassleepers(lock)==0)
	assert(lock->lockcurthread!=NULL);  
    splx(spl);

	// add stuff here as needed
	kfree(lock->name);
	//kfree(lock->lockcurthread);   
	kfree(lock);
	lock=NULL;
}

void
lock_acquire(struct lock *lock)
{
	
    // Write this
    int spl;
    assert(lock!=NULL);
    //assert(in_interrupt!=NULL);
    spl=splhigh();
    while(lock->lockcurthread!=NULL){
            thread_sleep(lock);
        }
    assert(lock->lockcurthread!=curthread);
    kprintf("\nlock acquired:: %s",lock->name);
    lock->lockcurthread=curthread;
    splx(spl);
	//(void)lock;  
	// suppress warning until code gets written
	//while(&lock->lockvalue){}	
}

void
lock_release(struct lock *lock)
{
	// Write this
	
	//(void)lock;  
	// suppress warning until code gets written
    int spl;
    assert(lock!=NULL);
    spl=splhigh();
    assert(lock->lockcurthread==curthread);
    kprintf("\nlock released:: %s",lock->name);
    lock->lockcurthread=NULL;
    thread_wakeup(lock);
    splx(spl);
}

int
lock_do_i_hold(struct lock *lock)
{
	// Write this

	//(void)lock;  
	// suppress warning until code gets written
	int retval=0;
	//spl=splhigh();
    	if(lock->lockcurthread==curthread)
		    retval=1; 
      	else 
		retval=0;
	//splx(spl);
	return retval;
	// dummy until code gets written
}

//Return the current value of the lock. If the current value is false
//set it to true so that the lock is aquired.
//If the current value is true, it is being used and hence keep waiting till it is released.
/*int
test_and_set(int *value){
	int temp = *value;
	*value=1;
	return temp;
}*/
////////////////////////////////////////////////////////////
//
// CV


struct cv *
cv_create(const char *name)
{
	struct cv *cv;

	cv = kmalloc(sizeof(struct cv));
	if (cv == NULL) {
		return NULL;
	}

	cv->name = kstrdup(name);
	if (cv->name==NULL) {
		kfree(cv);
		return NULL;
	}
	
	// add stuff here as needed
	
	return cv;
}

void
cv_destroy(struct cv *cv)
{
	assert(cv != NULL);

	// add stuff here as needed
	
	kfree(cv->name);
	kfree(cv);
}

void
cv_wait(struct cv *cv, struct lock *lock)
{
	// Write this
	//(void)cv;    
	// suppress warning until code gets written
	//(void)lock;  
	// suppress warning until code gets written

	assert(in_interrupt==0);	
	int spl;
	spl=splhigh();
	lock_release(lock);
	thread_sleep(cv);
	lock_acquire(lock);
	splx(spl);
}

void
cv_signal(struct cv *cv, struct lock *lock)
{
	// Write this
	//(void)cv;    
	// suppress warning until code gets written
	//(void)lock;  
	// suppress warning until code gets written
	int spl;
	spl=splhigh();
	thread_wakeup_one(cv);
	splx(spl);
}

void
cv_broadcast(struct cv *cv, struct lock *lock)
{
	// Write this
	//(void)cv;   
	// suppress warning until code gets written
	//(void)lock; 
	// suppress warning until code gets written
	int spl;
	spl=splhigh();
	thread_wakeup(cv);
	splx(spl);
}
