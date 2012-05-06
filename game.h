#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

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
	char map[MAPH][MAPW];
	struct resources_map resources_map[MAPH][MAPW];

	int goal_x, goal_y;
};

struct player {
	int x, y;
	int antidotes;
	char direction; /* n, e, s, w */
	int ammo;

	int bites;
	int points;
};

extern struct game_info GI;
extern struct player player;

void game_read(FILE *file);
void game_dump(void);
int game_check_border(int x, int y);

#endif /* GAME_H */
