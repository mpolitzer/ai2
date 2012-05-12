#include "game.h"
#include <math.h>

#define MOVE_FRAMES 10

static void draw_player(int scale);

static inline void gfx_render(int frame)
{
	int i;
	int scale = 38;

	al_clear_to_color(al_map_rgb(0,0,0));
	/* tiles */
	for (i=0; i<MAPH; i++) {
		int j;
		for (j=0; j<MAPW; j++) {
			ALLEGRO_COLOR color = al_map_rgb( 80, 80, 80);	/* R  */
			if (GI.map[j][i] == MAP_WALL)
				color = al_map_rgb( 5, 5, 5);	/* X  */

			al_draw_filled_rectangle(scale*i, scale*j,
			                         scale*(i + 1), scale*(j + 1),
			                         color);
		}
	}

	/* hospitals */
	for (i=0; i<GI.num_hospitals; i++) {
		float x = GI.hospitals[i].x + 0.5;
		float y = GI.hospitals[i].y + 0.5;

		if (GI.hospitals[i].antidotes)
			al_draw_scaled_bitmap(GI.medkit,
					0, 0,
					298,296,	/* image size. */
					scale*(x-0.5), scale*(y-0.5),
					scale, scale,
					0);
	}

	/* zombies */
	for (i=0; i<GI.num_zombies; i++) {
		float x = GI.zombies[i].x + 0.5;
		float y = GI.zombies[i].y + 0.5;

		if (GI.zombies[i].num)
			al_draw_scaled_bitmap(GI.zombie,
					0, 0,
					133, 124,	/* image size. */
					scale*(x-0.5), scale*(y-0.5),
					scale, scale,
					0);
	}

	/* stations */
	for (i=0; i<GI.num_stations; i++) {
		float x = GI.stations[i].x + 0.5;
		float y = GI.stations[i].y + 0.5;

		if (GI.stations[i].ammo)
			al_draw_scaled_bitmap(GI.ammo,
					0, 0,
					423, 336,	/* image size. */
					scale*(x-0.5), scale*(y-0.5),
					scale, scale,
					0);
	}

	/* end */
	al_draw_filled_circle(scale * (GI.goal_x+0.5),
	                      scale * (GI.goal_y+0.5), scale/2,
	                      al_map_rgb(  0,  0,255));
	draw_player(scale);
	al_flip_display();
}

static void draw_player(int scale)
{
	int x = player.x, y = player.y;

	al_draw_scaled_rotated_bitmap(GI.pacman,
			541.0/2, 600.0/2,
			scale*(x+0.5), scale*(y+0.5),
			scale/541.0, scale/600.0,
			player.direction*(M_PI/2)-M_PI/2,
			0);
}

void gfx_init(int w, int h, float fps)
{
	if (!al_init())
		die(1, "can't init allegro");
	if (!(al_init_primitives_addon()))
		die(2, "can't init primitives addon");
	if (!(GI.display = al_create_display(w, h)))
		die(3, "can't create display");
	if (!(GI.evQ = al_create_event_queue()))
		die(4, "can't create event queue");
	if (!(GI.tick = al_create_timer(1.0 / (fps*MOVE_FRAMES))))
		die(5, "tick died, invalid fps");
	if (!al_init_image_addon())
		die(6, "failed to load image addon");

	GI.medkit = al_load_bitmap("images/medkit.png");
	GI.pacman = al_load_bitmap("images/pacman.png");
	GI.zombie = al_load_bitmap("images/zombie.png");
	GI.ammo = al_load_bitmap("images/ammo.png");
	/* current position is the begining */

	al_register_event_source(GI.evQ, al_get_display_event_source(GI.display));
	al_register_event_source(GI.evQ, al_get_timer_event_source(GI.tick));
	al_start_timer(GI.tick);
}

void gfx_step(void)
{
	int i;

	for (i = 0; i < MOVE_FRAMES; i++) {
		ALLEGRO_EVENT ev;
		gfx_render(i);
		do {
			al_wait_for_event(GI.evQ, &ev);
		} while (ev.type != ALLEGRO_EVENT_TIMER);
	}
}

