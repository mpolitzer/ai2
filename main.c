#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <SWI-Prolog.h>

#include "game.h"

int main(int argc, char *argv[])
{
	game_read(stdin);
	game_dump();

	if (!PL_initialise(argc, argv)) {
		fprintf(stderr, "failed to initialize prolog\n");
		exit(1);
	}

	return 0;
}
