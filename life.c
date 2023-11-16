#include "grid.h"
#include "worker.h"
#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

char *foreground;
char *background;

int main(int argc, char* argv[])
{
	if(argc != 2) {
		fprintf(stderr, "Błędna licbza parametrów\n");
		return 1;
	}

	errno = 0;
    char* end;
    int number_workers = strtol(argv[1], &end, 0);
    if(*end != '\0' || errno != 0) {
        perror("Błąd odczytania liczby");
        return 1;
    }


	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode

	foreground = create_grid();
	background = create_grid();
	char *tmp;

	init_grid(foreground);
	pthread_t* workers = init_workers(number_workers);

	while (true)
	{
		draw_grid(foreground);
		

		// Step simulation
		update_grid(foreground, background, workers, number_workers);
		usleep(500 * 1000);
		tmp = foreground;
		foreground = background;
		background = tmp;
	}

	endwin(); // End curses mode
	destroy_grid(foreground);
	destroy_grid(background);
	destroy_workers(workers);

	return 0;
}
