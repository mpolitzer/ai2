#include <SWI-Prolog.h>
#include "game.h"

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
static foreign_t action_turn_chopper_on(void);

/* -------------------------------------------------------------------------- */
/*                                 SENSE                                      */
/* -------------------------------------------------------------------------- */
static foreign_t sense_zombies(term_t num)
{
	int i, count;
	int dir_vect[][2] = {
		{ 0,-1}, /* n */
		{ 1, 0}, /* e */
		{ 0, 1}, /* s */
		{-1, 0}, /* w */
		{ 0, 0}  /* current */
	};

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
	int dir_vect[][2] = {
		{ 0,-1}, /* n */
		{ 1, 0}, /* e */
		{ 0, 1}, /* s */
		{-1, 0}, /* w */
		{ 0, 0}  /* current */
	};

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
	int dir_vect[][2] = {
		{ 0,-1}, /* n */
		{ 1, 0}, /* e */
		{ 0, 1}, /* s */
		{-1, 0}, /* w */
		{ 0, 0}  /* current */
	};

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

static foreign_t sense_hit(void);

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

	if (!PL_unify_integer(X, player.x)) return FALSE;
	if (!PL_unify_integer(Y, player.y)) return FALSE;
	return TRUE;
}

static foreign_t consult_direction(term_t Dir)
{
	char *atom;
	char s[2] = { player.direction, '\0' };

	if (PL_is_variable(Dir))
		return PL_unify_atom_chars(Dir, s);

	PL_get_atom_chars(Dir, &atom);
	if (atom[0] == player.direction)
		return TRUE;
	return FALSE;
}

static foreign_t consult_goal(term_t X, term_t Y)
{
	if (!PL_unify_integer(X, GI.goal_x)) return FALSE;
	if (!PL_unify_integer(Y, GI.goal_y)) return FALSE;
	return TRUE;
}

/* -------------------------------------------------------------------------- */
/*                                ACTION                                      */
/* -------------------------------------------------------------------------- */

static foreign_t action_move_forward(void);
static foreign_t action_turn_right(void);
static foreign_t action_grab(void);
static foreign_t action_shoot(void);
static foreign_t action_turn_chopper_on(void);

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
#if 0
	PL_register_foreign("sense_hit", 0, sense_hit, 0);
#endif
	PL_register_foreign("consult_antidotes", 1, consult_antidotes, 0);
	PL_register_foreign("consult_ammo", 1, consult_ammo, 0);
	PL_register_foreign("consult_bites", 1, consult_bites, 0);
	PL_register_foreign("consult_position", 2, consult_position, 0);
	PL_register_foreign("consult_direction", 1, consult_direction, 0);
	PL_register_foreign("consult_goal", 2, consult_goal, 0);
#if 0
	PL_register_foreign("action_move_forward", 0, action_move_forward, 0);
	PL_register_foreign("action_turn_right", 0, action_turn_right, 0);
	PL_register_foreign("action_grab", 0, action_grab, 0);
	PL_register_foreign("action_shoot", 0, action_shoot, 0);
	PL_register_foreign("action_turn_chopper_on", 0, action_turn_chopper_on, 0);
#endif
}
