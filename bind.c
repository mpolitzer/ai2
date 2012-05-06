#include <SWI-Prolog.h>
#include "game.h"

static foreign_t sense_zombies(term_t num);
static foreign_t sense_police_station(void);
static foreign_t sense_hospital(void);
static foreign_t sense_antidote(void);
static foreign_t sense_ammo(void);
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
static foreign_t sense_zombies(term_t num);
static foreign_t sense_police_station(void);
static foreign_t sense_hospital(void);
static foreign_t sense_antidote(void);
static foreign_t sense_ammo(void);
static foreign_t sense_hit(void);

/* -------------------------------------------------------------------------- */
/*                                CONSULT                                     */
/* -------------------------------------------------------------------------- */


static foreign_t consult_antidotes(term_t t0)
{
	if (!PL_unify_integer(t0, player.antidotes)) return FALSE;
	return TRUE;
}

static foreign_t consult_ammo(term_t t0)
{
	if (!PL_unify_integer(t0, player.ammo)) return FALSE;
	return TRUE;
}

static foreign_t consult_bites(term_t t0)
{
	PL_unify_integer(t0, player.bites);
}

static foreign_t consult_position(term_t X, term_t Y)
{
	if (!PL_unify_integer(X, player.x)) return FALSE;
	if (!PL_unify_integer(Y, player.y)) return FALSE;
	return TRUE;
}

static foreign_t consult_direction(term_t Dir)
{
	char s[2] = "?";		/* terminate with NULL. */
	s[0] = player.direction;
	
	PL_unify_atom_chars(Dir, s);
	return TRUE;
}

static foreign_t consult_goal(term_t X, term_t Y)
{
	if (!PL_unify_integer(X, GI.goal_x)) return FALSE;
	if (!PL_unify_integer(Y, GI.goal_y)) return FALSE;
	return TRUE;
}


void register_binds(void)
{
#if 0
	PL_register_foreign("sense_zombies", 1, sense_zombies, 0);
	PL_register_foreign("sense_police_station", 0, sense_police_station, 0);
	PL_register_foreign("sense_hospital", 0, sense_hospital, 0);
	PL_register_foreign("sense_antidote", 0, sense_antidote, 0);
	PL_register_foreign("sense_ammo", 0, sense_ammo, 0);
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
