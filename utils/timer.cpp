#include "timer.hpp"

Timer::Timer()
{
  _Ts = -1;
}

void Timer::init(double dt)
{
  //Initializes the timer, input is the desired timestep in SECONDS
  if(dt>0){ //Make sure timestep is valid

    // Build Semaphore Names
    sem_init(&sem_timer, 0, 0);
    // Initialize timer
    clock_gettime(CLOCK_REALTIME, &timestops);
    timestopd.tv_sec  = timestops.tv_sec;
    timestopd.tv_nsec = 0;

    _Ts = dt;
    count_me = 0;
  }else{
    printf("Error in Timer::init : Invalid timestep specified./n");
    throw 1;
  }
}

void Timer::wait()
{
  if(_Ts <= 0){ //Make sure the timer is initialized

    printf("Error in Timer::wait : Must initialize the timer first using Timer::init/n");
    throw 1;
  }

  count_me++;
  if (timestopd.tv_nsec > (int) (1000000000*((1/_Ts - 2)*_Ts))){
    timestopd.tv_sec++;
    timestopd.tv_nsec = 0;
    //printf("I ran %d cycles in 1 sec \n", count_me);
    count_me = 0;
  }
  else{
    timestopd.tv_nsec +=   (int) (1000000000*_Ts);
  }
  sem_timedwait(&sem_timer, &timestopd);

}
