#include <stdio.h>
#include <SWI-Prolog.h>
#include "game.h"

struct game_info GI;
struct player player;

void clear_screen_and_home(void)
{
	printf("\033[2J\033[0;0H");
}

void putcolor(char c, int fg, int bg)
{
	printf("\033[%d;%dm %c \033[0m", fg, bg, c);
}

void game_read(FILE *file)
{
	char c;
	int i,j,x,y,z;

	while (fscanf(file, " %c:%d,%d,%d", &c, &x, &y, &z) == 4) {
		x -= 1;
		y -= 1;
		switch (c) {
			case 'H': case 'h':
				GI.hospitals[GI.num_hospitals].x = x;
				GI.hospitals[GI.num_hospitals].y = y;
				GI.hospitals[GI.num_hospitals].antidotes = z;

				GI.resources_map[y][x].what = IS_HOSPITAL;
				GI.resources_map[y][x].hospital =
					&GI.hospitals[GI.num_hospitals];
				GI.num_hospitals++;
				break;
			case 'M': case 'm':
				GI.stations[GI.num_stations].x = x;
				GI.stations[GI.num_stations].y = y;
				GI.stations[GI.num_stations].ammo = z;

				GI.resources_map[y][x].what = IS_STATION;
				GI.resources_map[y][x].station =
					&GI.stations[GI.num_stations];

				GI.num_stations++;
				break;
			case 'Z': case 'z':
				GI.zombies[GI.num_zombies].x = x;
				GI.zombies[GI.num_zombies].y = y;
				GI.zombies[GI.num_zombies].num = z;

				GI.resources_map[y][x].what = IS_ZOMBIE;
				GI.resources_map[y][x].zombie =
					&GI.zombies[GI.num_zombies];

				GI.num_zombies++;
				break;
		}
	}
	ungetc(c, file);

	for (i=0; i<MAPH; i++) {
		char buf[255];

		fgets(buf, 255, file);

		for (j=0; !isspace(buf[j]); j++) {
			GI.map[i][j] = toupper(buf[j]);
		}
	}
}

void game_dump(void)
{
	int i,j;

	clear_screen_and_home();
	for (i=0; i<MAPH; i++) {
		for (j=0; j<MAPW; j++) {
			struct resources_map *rm = &GI.resources_map[i][j];
			if (rm->what == IS_ZOMBIE) {
				putcolor('Z', 30, 46);
				continue;
			}
			if (rm->what == IS_HOSPITAL) {
				putcolor('H', 30, 41);
				continue;
			}
			if (rm->what == IS_STATION) {
				putcolor('M', 37, 43);
				continue;
			}

			switch (GI.map[i][j]) {
			case 'X' : case 'x':
				putcolor(' ', 31, 40);
				break;
			case 'R': case 'r':
				putcolor(' ', 30, 47);
				break;
			}
		}
		printf("\n");
	}
}

int game_check_border(int x, int y)
{
	if (x < 0) return 0;
	if (y < 0) return 0;
	if (x >= MAPW) return 0;
	if (y >= MAPH) return 0;

	return 1;
}

void game_update_points(int action)
{
	int points_vet[] = {-1, -1, -1, -10, -1, -100, 20, 200};
	int factor = 0;

	if(action == ACTION_FUCK_YEAH)
	{
		factor += player.antidotes;
	}
	else
	{
		factor += player.bites+1;
	}
	
	player.points += points_vet[action] * factor;
}

void game_init(void)
{
	GI.goal_x = 17;
	GI.goal_y = 16;
	player.x = 2;
	player.y = 2;
	player.ammo = 30;
	player.antidotes = 1;
	player.direction = DIR_E;
}
