#pragma once
#include <sys/types.h>
#include <bits/pthreadtypes.h>
#include <stdbool.h>

char *create_grid();
void destroy_grid(char *grid);
void draw_grid(char *grid);
void init_grid(char *grid);
bool is_alive(int row, int col, char *grid);
void update_grid(char *src, char *dst, pthread_t* workers, int workers_length);