#ifndef THREAD_HPP_
#define THREAD_HPP_

#include <sys/time.h>
#include <sys/select.h>
#include <semaphore.h>
#include <signal.h>
#include <iostream>
#include <math.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <algorithm>



// ***************************************************
//## Threading stuff - Buddy Michini
// ***************************************************
/*
  A struct for storing the Object pointer(o) and a function parameter(p)
  T is a pointer the class
  P is a pointer to the parameter which is passed to the class memeber function
*/
template<typename T,typename P>
struct OP
{
  T o;
  P p;
};

//normal posix pthread C function pointer
typedef void* (*func_ptr) (void*);

/*
  posix class member function starter
  R is the return type of the member function
  T is the type of class which the function is a member function of
  P is the member functions parameter- NOTE THIS IS A POINTER we are dealing with
  passing information between threads
  R(T::*f)(P) is the signture of the function pointer
  void* op is the templates struct above OP<T,P>
*/
template<class R,class T,class P, R (T::*f)(P)>
void* thread_starter(void* op)
{
  //get and store the class pointer
  T* p_class = static_cast<T*>(static_cast<OP<T*,P>*>(op)->o);
  //get and store the parameter
  P parm = static_cast<P>(static_cast<OP<T*,P>*>(op)->p);
  //delete the struct which contains the class pointer and parameter
  delete static_cast<OP<T*,P>*>(op);
  //call the class member function passing the parameter
  (p_class->*f)(parm);
  return 0;
}

/*
  Class : Thread
  Remarks :A simple inlined wrapper for pthread
*/
class Thread
{
public:
  Thread( void ){;}
  ~Thread( void ){;}
  inline bool start( func_ptr func, void* data)
  {
    struct sched_param params;
    pthread_attr_t thread_attr;

    // Set priority
    params.sched_priority  = 20;
    // Initialize Thread Attributes
    pthread_attr_init(&thread_attr);
    // Set the thread attributes to detached (so they yield back any memory etc on their own...
    //pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    //NOTE: I commented out the detached state (leaving it joinable) because i'd like to be able to exit threads cleanly with join and a close flag
    // Setup default scheduling
    pthread_attr_setinheritsched (&thread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy (&thread_attr, SCHED_FIFO);
    pthread_attr_setschedparam (&thread_attr, &params);

    if( pthread_create( &m_thread, &thread_attr, func, data ) != 0){ return false; }

    return true;
  }

  template<class R, class T,class P, R (T::*mem_func)(P)>
  inline bool start_member(T *object, P param)
  {
    OP<T*,P>* op = new OP<T*,P>;
    op->o=object;
    op->p=param;

    struct sched_param params;
    pthread_attr_t thread_attr;

    // Set priority
    params.sched_priority  = 20;
    // Initialize Thread Attributes
    pthread_attr_init(&thread_attr);
    // Set the thread attributes to detached (so they yield back any memory etc on their own...
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    // Setup default scheduling
    pthread_attr_setinheritsched (&thread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy (&thread_attr, SCHED_FIFO);
    pthread_attr_setschedparam (&thread_attr, &params);

    if( pthread_create( &m_thread, &thread_attr, thread_starter<R,T,P,mem_func>, static_cast<void*>(op) ) != 0)
      { return false; }

    return true;
  }

  inline bool join( void )
  {
  	int res = pthread_join(m_thread, NULL);
  	switch(res){
  		case EINVAL:
  			printf("PTHREAD ERROR: Join() returned EINVAL.");
  			return false;
  			break;
  		case ESRCH:
  			printf("PTHREAD ERROR: Join() returned ESRCH.");
  			return false;
  			break;
  		case EDEADLK:
  			printf("PTHREAD ERROR: Join() returned EDEADLK.");
  			return false;
  			break;	
  		default :
  			return true;
  	}
  }
  
  inline bool close_thread( void )
  {
    int result = pthread_cancel(m_thread);
    if (result != 0){ return false; }
    return true;
  }
  inline void exit_caller( void )
  {
    int exit_code = -1;
    pthread_exit(&exit_code);
  }
private:
  pthread_t m_thread;
};


#endif /*THREAD_HPP_*/
