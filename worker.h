#pragma once
#include <sys/types.h>
#include <bits/pthreadtypes.h>

pthread_t create_worker(int x, int y);
void step(pthread_t threadid);
pthread_t* init_workers(int number);
void destroy_workers(pthread_t* workers);