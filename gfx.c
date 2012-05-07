#include "game.h"

#define MOVE_FRAMES 10

static inline void gfx_render(int frame)
{
	int i;
	int scale = 20;
	float render_pos[2];

	al_clear_to_color(al_map_rgb(0,0,0));
	/* tiles */
	for (i=0; i<MAPH; i++) {
		int j;
		for (j=0; j<MAPW; j++) {
			ALLEGRO_COLOR color = al_map_rgb( 50,   0,  0);	/* R  */
			if (GI.map[j][i] == MAP_WALL)
				color = al_map_rgb(  0,   0,  0);	/* X  */

			al_draw_filled_rectangle(scale*i, scale*j,
			                         scale*(i + 1), scale*(j + 1),
			                         color);
		}
	}

#if 0
	/* prize */
	for (i=0; i<G.gi.num_prizes; i++) {
		float x = G.gi.prizes[i][0] + 0.5;
		float y = G.gi.prizes[i][1] + 0.5;
		al_draw_filled_triangle(scale*(x), scale*(y-0.3),
		                        scale*(x+0.3), scale*(y+0.3),
		                        scale*(x-0.3), scale*(y+0.3),
		                        al_map_rgb(255, 255, 0));
	}
	/* end */
	al_draw_filled_circle(scale * (G.gi.end[0]+0.5),
	                      scale * (G.gi.end[1]+0.5), scale/2,
	                      al_map_rgb(255, 0, 0));
	render_pos[0]=prev_pos[0]+(float)frame*(pos[0] - prev_pos[0])/MOVE_FRAMES;
	render_pos[1]=prev_pos[1]+(float)frame*(pos[1] - prev_pos[1])/MOVE_FRAMES;
#endif

	render_pos[0] = player.x;
	render_pos[1] = player.y;

	/* player */
	al_draw_filled_circle(scale * (render_pos[0]+0.5),
			scale * (render_pos[1]+0.5), scale/2,
			al_map_rgb(  0,  0,255));
	al_flip_display();
}


void gfx_init(int w, int h, int fps)
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

	/* current position is the begining */

	al_register_event_source(GI.evQ, al_get_display_event_source(GI.display));
	al_register_event_source(GI.evQ, al_get_timer_event_source(GI.tick));
	al_start_timer(GI.tick);
}

void gfx_step(void)
{
	int i;
	ALLEGRO_EVENT ev;

	for (i = 0; i < MOVE_FRAMES; i++) {
		gfx_render(i);
		do {
			al_wait_for_event(GI.evQ, &ev);
		} while (ev.type != ALLEGRO_EVENT_TIMER);
	}
}

