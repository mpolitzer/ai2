#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#define NUM_HOSPITALS	4
#define NUM_STATIONS	4
#define NUM_ZOMBIES	60

#define MAPW 20
#define MAPH 20

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

struct hospital {
	int x, y;
	int antidotes;
};

struct station {
	int x, y;
	int ammo;
};

struct zombie {
	int x, y;
	int num;
};

struct resources_map {
#define IS_NOTHING	0
#define IS_HOSPITAL	1
#define IS_STATION	2
#define IS_ZOMBIE	3
	int what;

	union {
		struct hospital *hospital;
		struct station *station;
		struct zombie *zombie;
	};
};

struct game_info {
	struct hospital hospitals[NUM_HOSPITALS];
	struct station stations[NUM_STATIONS];
	struct zombie zombies[NUM_ZOMBIES];
	int num_hospitals;
	int num_stations;
	int num_zombies;

#define MAP_WALL	'X'
#define MAP_ROAD	'R'
	char map[MAPH][MAPW];
	struct resources_map resources_map[MAPH][MAPW];

	int goal_x, goal_y;

	int was_hit;

	ALLEGRO_DISPLAY *display;

	ALLEGRO_TIMER *tick;
	ALLEGRO_EVENT_QUEUE *evQ;


};

struct player {
	int x, y;
	int antidotes;

#define DIR_N	0
#define DIR_E	1
#define DIR_S	2
#define DIR_W	3
#define DIR_C	4
	int direction; /* n, e, s, w, curr */
	int ammo;

	int bites;
	int points;
};

extern struct game_info GI;
extern struct player player;

enum {
	ACTION_WALK,
	ACTION_TURN_RIGHT,
	ACTION_GRAB,
	ACTION_SHOOT,
	ACTION_TURN_CHOPPER_ON,
	ACTION_GET_BITTEN,
	ACTION_KILL_ZOMBIE,
	ACTION_FUCK_YEAH /* escape */
};

void game_update_points(int action);
void game_read(FILE *file);
void game_init(void);
void game_dump(void);
int game_check_border(int x, int y);

#define die(error, ...) _die(error, __FILE__, __LINE__, __VA_ARGS__)
void _die(int error, char* filename, int line, const char* format, ...);

void gfx_init(int w, int h, int fps);
void gfx_step(int x, int y);

#endif /* GAME_H */
