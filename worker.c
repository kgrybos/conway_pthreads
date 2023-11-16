#include "grid.h"
#include <sys/types.h>
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

extern char* foreground;
extern char* background;
extern const int grid_width;
extern const int grid_height;

struct coordinates {
    int x;
    int y;
};

struct coordinates_array {
    int length;
    struct coordinates coordinates[];
};

void signal_handler(int signum) {}

void* worker(void* args) {
    struct coordinates_array* coords = (struct coordinates_array*) args;
    while(1) {
        pause();
        
        for(int i = 0; i < coords->length; i++) {
            int x = coords->coordinates[i].x;
            int y = coords->coordinates[i].y;
            // printf("%d %d %d\n", coords->coordinates[i].x, coords->coordinates[i].y, is_alive(y, x, foreground));
            background[y * grid_width + x] = is_alive(y, x, foreground);
        }
    }
}

pthread_t create_worker(struct coordinates_array* coords) {
    pthread_t new_worker;
    pthread_create(&new_worker, NULL, worker, coords);
    return new_worker;
}

void step(pthread_t threadid) {
    pthread_kill(threadid, SIGUSR1);
}

pthread_t* init_workers(int number) {
    pthread_t* workers = malloc(number * sizeof(pthread_t));

    signal(SIGUSR1, signal_handler);

    int number_per_worker = (grid_width*grid_height) / number;
    int remainder = (grid_width*grid_height) % number;

    for (int i = 0; i < number; i++) {
        struct coordinates_array* coords = malloc(sizeof(struct coordinates_array) + grid_width*grid_height*sizeof(struct coordinates));
        if(coords == NULL) {
            perror("Błąd alokacji pamięci");
            return NULL;
        }
        int start = i*number_per_worker;

        if(i == number-1) {
            number_per_worker += remainder;
        }
        coords->length = number_per_worker;
        for(int j = 0; j < number_per_worker; j++) {
            coords->coordinates[j].x = (start+j) % grid_width;
            coords->coordinates[j].y = (start+j) / grid_width;
        }

        workers[i] = create_worker(coords);
    }

    return workers;
}

void destroy_workers(pthread_t* workers) {
    for(int i = 0; i < grid_width*grid_height; i++) {
        pthread_cancel(workers[i]);
    }

    free(workers);
}