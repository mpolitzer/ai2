#include <SWI-Prolog.h>
#include "game.h"

static int dir_vect[][2] = {
	{ 0,-1}, /* n */
	{ 1, 0}, /* e */
	{ 0, 1}, /* s */
	{-1, 0}, /* w */
	{ 0, 0}  /* current */
};

/* -------------------------------------------------------------------------- */
/*                                 PROTOTYPES                                 */
/* -------------------------------------------------------------------------- */
static foreign_t sense_zombies(term_t num);
static foreign_t sense_station(void);
static foreign_t sense_hospital(void);
static foreign_t sense_antidote(term_t t0);
static foreign_t sense_ammo(term_t t0);
static foreign_t sense_hit(void);

static foreign_t consult_antidotes(term_t num);
static foreign_t consult_ammo(term_t num);
static foreign_t consult_bites(term_t num);
static foreign_t consult_position(term_t X,term_t Y);
static foreign_t consult_direction(term_t Direction);
static foreign_t consult_goal(term_t X,term_t Y);

static foreign_t action_move_forward(void);
static foreign_t action_turn_right(void);
static foreign_t action_grab(void);
static foreign_t action_shoot(void);
static foreign_t action_use_antidote(void);
static foreign_t action_turn_chopper_on(void);

/* -------------------------------------------------------------------------- */
/*                                 SENSE                                      */
/* -------------------------------------------------------------------------- */
static foreign_t sense_zombies(term_t num)
{
	int i, count;

	if (!PL_is_variable(num))
		return FALSE;

	for (count=0, i=0; i<sizeof(dir_vect)/sizeof(*dir_vect); i++) {
		struct resources_map *rm;
		int x = player.x + dir_vect[i][1];
		int y = player.y + dir_vect[i][0];

		if (!game_check_border(x, y)) continue;

		rm = &GI.resources_map[y][x];
		if (rm->what == IS_ZOMBIE) {
			count += rm->zombie->num;
		}
	}
	
	PL_unify_integer(num, count);
	if (count)
		return TRUE;
	return FALSE;
}

static foreign_t sense_station(void)
{
	int i;

	for (i=0; i<sizeof(dir_vect)/sizeof(*dir_vect); i++) {
		struct resources_map *rm;
		int x = player.x + dir_vect[i][1];
		int y = player.y + dir_vect[i][0];

		if (!game_check_border(x, y)) continue;

		rm = &GI.resources_map[y][x];
		if (rm->what == IS_STATION)
			return TRUE;
	}
	return FALSE;
}

static foreign_t sense_hospital(void)
{
	int i;

	for (i=0; i<sizeof(dir_vect)/sizeof(*dir_vect); i++) {
		struct resources_map *rm;
		int x = player.x + dir_vect[i][1];
		int y = player.y + dir_vect[i][0];

		if (!game_check_border(x, y)) continue;

		rm = &GI.resources_map[y][x];
		if (rm->what == IS_HOSPITAL)
			return TRUE;
	}
	return FALSE;
}

static foreign_t sense_antidote(term_t t0)
{
	int x = player.x;
	int y = player.y;
	struct resources_map *rm;
	
	if (!PL_is_variable(t0))
		return FALSE;

	rm = &GI.resources_map[y][x];

	if (rm->what == IS_HOSPITAL)
		return PL_unify_integer(t0, rm->hospital->antidotes);
	return FALSE;
}

static foreign_t sense_ammo(term_t t0)
{
	int x = player.x;
	int y = player.y;
	struct resources_map *rm;
	
	if (!PL_is_variable(t0))
		return FALSE;

	rm = &GI.resources_map[y][x];

	if (rm->what == IS_STATION)
		return PL_unify_integer(t0, rm->station->ammo);
	return FALSE;
}

static foreign_t sense_hit(void)
{
	if(GI.was_hit)
	{
		GI.was_hit = 0;
		return TRUE;
	}

	return FALSE;
}

/* -------------------------------------------------------------------------- */
/*                                CONSULT                                     */
/* -------------------------------------------------------------------------- */


static foreign_t consult_antidotes(term_t t0)
{
	if (!PL_is_variable(t0))
		return FALSE;

	return PL_unify_integer(t0, player.antidotes);
}

static foreign_t consult_ammo(term_t t0)
{
	if (!PL_is_variable(t0))
		return FALSE;

	return PL_unify_integer(t0, player.ammo);
}

static foreign_t consult_bites(term_t t0)
{
	if (!PL_is_variable(t0))
		return FALSE;

	return PL_unify_integer(t0, player.bites);
}

static foreign_t consult_position(term_t X, term_t Y)
{
	if (!PL_is_variable(X) || !PL_is_variable(Y))
		return FALSE;

	if (!PL_unify_integer(X, player.x + 1)) return FALSE;
	if (!PL_unify_integer(Y, player.y + 1)) return FALSE;
	return TRUE;
}

static foreign_t consult_direction(term_t Dir)
{
	char *dir_name[] = {"n", "e", "s", "w"};
	char *atom;

	if (PL_is_variable(Dir))
		return PL_unify_atom_chars(Dir, dir_name[player.direction]);

	PL_get_atom_chars(Dir, &atom);
	if (atom[0] == dir_name[player.direction][0])
		return TRUE;
	return FALSE;
}

static foreign_t consult_goal(term_t X, term_t Y)
{
	if (!PL_unify_integer(X, GI.goal_x + 1)) return FALSE;
	if (!PL_unify_integer(Y, GI.goal_y + 1)) return FALSE;
	return TRUE;
}

/* -------------------------------------------------------------------------- */
/*                                ACTION                                      */
/* -------------------------------------------------------------------------- */

static foreign_t action_move_forward(void)
{
	struct resources_map *rm;
	int x = player.x + dir_vect[player.direction][0];
	int y = player.y + dir_vect[player.direction][1];

	if(!game_check_border(x, y))
		return FALSE;

	if(GI.map[y][x] == MAP_WALL)
		return FALSE;

	player.x = x;
	player.y = y;

	game_update_points(ACTION_WALK);

       	rm = &GI.resources_map[y][x];
	if(rm->what == IS_ZOMBIE)
	{
		game_update_points(ACTION_GET_BITTEN);
		player.bites += rm->zombie->num;
		rm->what = IS_NOTHING;
	}

	return TRUE;
}

static foreign_t action_turn_right(void)
{
	player.direction = (player.direction+1)%4;

	game_update_points(ACTION_TURN_RIGHT);

	return TRUE;
}

static foreign_t action_grab(void)
{
	struct resources_map *rm = &GI.resources_map[player.y][player.x];

	if(rm->what == IS_HOSPITAL)
	{
		player.antidotes += rm->hospital->antidotes;
		rm->what = IS_NOTHING;

		game_update_points(ACTION_GRAB);
		return TRUE;
	}
	
	if(rm->what == IS_STATION)
	{
		player.ammo += rm->station->ammo;
		rm->what = IS_NOTHING;

		game_update_points(ACTION_GRAB);
		return TRUE;
	}
	
	return FALSE;
}

static foreign_t action_shoot(void)
{
	struct resources_map *rm;

	int dx = dir_vect[player.direction][0];
	int dy = dir_vect[player.direction][1];
	int x = player.x, y = player.y;

	if(player.ammo == 0)
		return FALSE;
	player.ammo--;

	game_update_points(ACTION_SHOOT);
	while(game_check_border(x, y))
	{
		if(GI.map[y][x] == MAP_WALL)
			break;

		rm = &GI.resources_map[y][x];
		if(rm->what == IS_ZOMBIE)
		{
			rm->zombie->num--;

			if(rm->zombie->num == 0)
			{
				rm->what = IS_NOTHING;
			}

			GI.was_hit = 1;
	
			game_update_points(ACTION_KILL_ZOMBIE);
			return TRUE;
		}

		x += dx;
		y += dy;
	}

	return FALSE;
}

static foreign_t action_use_antidote(void)
{
	if(player.antidotes > 0)
	{
		player.antidotes--;
		player.bites = 0;

		return TRUE;
	}
	return FALSE;
}

static foreign_t action_turn_chopper_on(void)
{
	if(GI.goal_x != player.x || GI.goal_y != player.y)
		return FALSE;

	game_update_points(ACTION_TURN_CHOPPER_ON);
	game_update_points(ACTION_FUCK_YEAH);

	/* TODO: acaba a execucao */
	return TRUE;
}

/* -------------------------------------------------------------------------- */
/*                                 BIND                                       */
/* -------------------------------------------------------------------------- */

void register_binds(void)
{
	PL_register_foreign("sense_zombies", 1, sense_zombies, 0);
	PL_register_foreign("sense_station", 0, sense_station, 0);
	PL_register_foreign("sense_hospital", 0, sense_hospital, 0);
	PL_register_foreign("sense_antidote", 1, sense_antidote, 0);
	PL_register_foreign("sense_ammo", 1, sense_ammo, 0);
	PL_register_foreign("sense_hit", 0, sense_hit, 0);

	PL_register_foreign("consult_antidotes", 1, consult_antidotes, 0);
	PL_register_foreign("consult_ammo", 1, consult_ammo, 0);
	PL_register_foreign("consult_bites", 1, consult_bites, 0);
	PL_register_foreign("consult_position", 2, consult_position, 0);
	PL_register_foreign("consult_direction", 1, consult_direction, 0);
	PL_register_foreign("consult_goal", 2, consult_goal, 0);

	PL_register_foreign("action_move_forward", 0, action_move_forward, 0);
	PL_register_foreign("action_turn_right", 0, action_turn_right, 0);
	PL_register_foreign("action_grab", 0, action_grab, 0);
	PL_register_foreign("action_shoot", 0, action_shoot, 0);
	PL_register_foreign("action_use_antidote", 0, action_use_antidote, 0);
	PL_register_foreign("action_turn_chopper_on", 0, action_turn_chopper_on, 0);
}
