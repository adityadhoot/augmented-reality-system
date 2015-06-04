#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <stdio.h>
#include <sys/time.h>
#include <sys/select.h>
#include <semaphore.h>
#include <iostream>

class Timer
{
public:

  // constructors
  Timer();

  //Initializes the timer, input is the desired timestep in SECONDS
  void init(double dt);

  void wait();

private:
  double _Ts; // Timestep, in SECONDS
  sem_t sem_timer; // Semaphore for timer control

  // Timer Variables
  struct timespec timestops;
  struct timespec timestopd;

  int count_me;

};


#endif /*TIMER_HPP_*/
