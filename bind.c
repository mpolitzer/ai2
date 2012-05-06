#include <SWI-Prolog.h>

static foreign_t sense_zombies(term_t num);
static foreign_t sense_police_station(void);
static foreign_t sense_hospital(void);
static foreign_t sense_antidote(void);
static foreign_t sense_ammo(void);
static foreign_t sense_hit(void);

static foreign_t consult_altidotes(term_t num);
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

void register_binds(void);


static foreign_t consult_altidotes(term_t num);
