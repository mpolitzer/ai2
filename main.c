#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <SWI-Prolog.h>

#include "game.h"
#include "bind.h"

/*
 * TODO:
 * - matar o fela da puta na virada de turno
 *
 * */

int main(int argc, char *argv[])
{
	FILE *f = fopen("in.txt", "r");
	game_init();
	game_read(f);
	game_dump();

	register_binds();

	if (!PL_initialise(argc, argv)) {
		fprintf(stderr, "failed to initialize prolog\n");
		exit(1);
	}

	PL_halt(PL_toplevel() ? 0 : 1);
	return 0;
}
