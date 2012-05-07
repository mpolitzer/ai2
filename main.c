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

#define die(error, ...) _die(error, __FILE__, __LINE__, __VA_ARGS__)
void _die(int error, char* filename, int line, const char* format, ...)
{
	va_list args;

	fprintf(stderr, "[%s:%d] ", filename, line);
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	fprintf(stderr, "\n");
	exit(error);
}


int main(int argc, char *argv[])
{
	FILE *f = fopen("in.txt", "r");
	ALLEGRO_EVENT ev;

	gfx_init(600, 600, 10);
	game_init();
	game_read(f);

	register_binds();

	al_wait_for_event(GI.evQ, &ev);

	if (ev.type == ALLEGRO_EVENT_KEY_DOWN
			&& ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
		return -1;

	if (!PL_initialise(argc, argv)) {
		fprintf(stderr, "failed to initialize prolog\n");
		exit(1);
	}

	gfx_step(player.x, player.y);
	PL_halt(PL_toplevel() ? 0 : 1);
	return 0;
}
