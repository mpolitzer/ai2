
% ----------------------------------- %

:- dynamic 
	has_zombie/3, 
	curr_position/2, 
	has_antidote/2, 
	has_ammo/2, 
	safe_tile/2.

% ----------------------------------- %

curr_position(-1,-1).

load_position :-
	consult_position(X,Y),
	retract(curr_position(_,_)),
	assert(curr_position(X,Y)),
	assert(safe_tile(X,Y)).

% ----------------------------------- %

load_sense_zombies :-
	sense_zombies(N),
	true.
	
load_sense_station :-
	sense_station,
	true.

load_sense_hospital :-
	sense_hospital,
	true.

load_sense_antidote :-
	sense_antidote,
	curr_position(X,Y),
	has_antidote(X,Y).

load_sense_ammo :-
	sense_ammo(N),
	curr_position(X,Y),
	has_ammo(coord(X,Y), N).

load_sense_hit :-
	true.

init_sensors :-
	load_sense_zombies,
	load_sense_station,
	load_sense_hospital,
	load_sense_antidote,
	load_sense_ammo,
	load_sense_hit.

% ----------------------------------- %

logic :-
	true.

% ----------------------------------- %

strategy :-
	load_position,
	init_sensors,
	logic.

% ----------------------------------- %


