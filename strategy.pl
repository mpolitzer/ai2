% TODO: Remove this HACK %

%sense_zombies
%sense_station
%sense_hospital
%sense_antidote
%sense_ammo
%sense_hit
%
%consult_antidotes
%consult_ammo
%consult_bites
%consult_position
%consult_direction
%consult_goal

f :- move_forward.
r :- action_turn_right.
l :-	action_turn_right,
	action_turn_right,
	action_turn_right.
g :- action_grab.
s :- action_shoot.
ua :- action_use_antidote.
on :- action_turn_chopper_on.
sz(N) :- sense_zombies(N).

st(N) :- N = 0.
st(N) :- strategy,X is N-1,st(X).

% ----------------------------------- %

:- dynamic
	has_zombie/3,
	safe_tile/2,
	visited/2,
	visited_list/2.

% ----------------------------------- %

dir_vect(n, vect(0,-1)).
dir_vect(e, vect(1,0)).
dir_vect(s, vect(0,1)).
dir_vect(w, vect(-1,0)).

next_position(X,Y) :-
	consult_position(OldX,OldY),
	consult_direction(D),
	dir_vect(D, vect(DX,DY)),
	X is OldX+DX,
	Y is OldY+DY.

next_position_d(X, Y, D) :-
	consult_position(OldX,OldY),
	dir_vect(D, vect(DX,DY)),
	X is OldX+DX,
	Y is OldY+DY.

turn_to(D) :-
	(
		consult_direction(OldD),
		not(D = OldD),
		action_turn_right,
		turn_to(D)
	);
	true.

% ----------------------------------- %

move_to_resource :-
	next_position(X,Y),
	(
		not(is_wall(X,Y));
		is_border(X,Y)
	),
	action_turn_right,
	move_to_resource.

move_to_resource :-
	next_position(_,_),
	(
		(
			not(move_forward),
			action_turn_right,
			move_to_resource
		);
		action_grab
	).

% ----------------------------------- %

set_safe_tiles :-
	(consult_position(X,Y),
	dir_vect(_,vect(DX,DY)),
	NX is X+DX,
	NY is Y+DY,
	not(is_wall(NX,NY)),
	not(is_border(NX,NY)),
	not(sense_zombies(_)),
	assert(safe_tile(NX,NY)));true.

% ----------------------------------- %

has_unvis(D) :-
	consult_position(X,Y),
	dir_vect(D,vect(DX,DY)),
	NX is X+DX, NY is Y+DY,
	not(is_wall(NX,NY)),
	not(is_border(NX,NY)),
	not(visited(NX,NY)).

visited_list([], []).

move_forward :-
	consult_position(OldX,OldY),
	next_position(X,Y),
	action_move_forward,
	assert(visited(X,Y)),
	retract(visited_list(LX, LY)),
	assert(visited_list([OldX|LX],[OldY|LY])),
	set_safe_tiles.

% ----------------------------------- %

vect_to_dir(X, Y, D1, D2) :-
	abs(Y,AY),
	X > AY,
	D1 = e,
	(
		(
			Y > 0,
			D2 = s
		);
		D2 = n
	).
vect_to_dir(X, Y, D1, D2) :-
	abs(Y,AY),
	-X > AY,
	D1 = w,
	(
		(
			Y > 0,
			D2 = s
		);
		D2 = n
	).

vect_to_dir(X, Y, D1, D2) :-
	abs(X,AX),
	Y > AX,
	D1 = s,
	(
		(
			X > 0,
			D2 = e
		);
		D2 = w
	).

vect_to_dir(X, Y, D1, D2) :-
	abs(X,AX),
	-Y > AX,
	D1 = n,
	(
		(
			X > 0,
			D2 = e
		);
		D2 = w
	).


% ----------------------------------- %

mark_safe_by_shoot(X,Y) :-
	is_wall(X,Y);
	is_border(X,Y).

mark_safe_by_shoot(XI,YI) :-
	consult_direction(D),
	dir_vect(D,DX,DY),
	X is XI+DX,
	Y is YI+DY,
	assert(safe_tile(X,Y)),
	mark_safe_by_shoot(X, Y).

% ----------------------------------- %


strategy :-
	consult_position(X,Y),
	consult_goal(GX,GY),
	X = GX,
	Y = GY,
	action_turn_chopper_on.

strategy :-
	(
		sense_station;
		sense_hospital
	),
	move_to_resource.

strategy :-
	consult_position(X,Y),
	consult_goal(GX,GY),
	DX is GX-X, DY is GY-Y,
	dir_vect(D,vect(DX,DY)),
	turn_to(D),
	move_forward.

strategy :-
	consult_bites(N),
	N > 0,
	action_use_antidote.


strategy :-
	sense_zombies(N),
	safe_tile(_,_),
	dfs.

strategy :-
	sense_zombies(N),
	next_position(X,Y),
	not(is_wall(X,Y)),
	((
		action_shoot,
		(
			(
				sense_zombies(NN),
				NN = N,
				move_forward
			);
			true
		)
	);(
		not(sense_hit),
		mark_safe_by_shoot(X,Y)
	)).

strategy :-
	not(has_unvis(_)),
	dfs.

dfs :-
	retract(visited_list([X|LX], [Y|LY])),
	assert(visited_list(LX,LY)),
	consult_position(CurrX, CurrY),
	DX is X-CurrX, DY is Y-CurrY,
	abs(DX,AX),
	abs(DY,AY),
	assertion(AX < 2),
	assertion(AY < 2),
	assertion(AX > 0;AY > 0),
	dir_vect(D,vect(DX,DY)),
	turn_to(D),
	action_move_forward.

strategy :-
	consult_position(CurrX, CurrY),
	consult_goal(GoalX, GoalY),
	DX is GoalX-CurrX,
	DY is GoalY-CurrY,
	vect_to_dir(DX,DY,D1,D2),
	(
		(
			next_position_d(X,Y,D1),
			not(is_wall(X,Y)),
			not(is_border(X,Y)),
			not(visited(X,Y)),
			turn_to(D1)
		);(
		next_position_d(X,Y,D2),
		not(is_wall(X,Y)),
		not(is_border(X,Y)),
		not(visited(X,Y)),
		turn_to(D2)
	)
),
move_forward.

strategy :-
	next_position(X,Y),
	(
		is_wall(X,Y);
		is_border(X,Y);
		visited(X,Y)
	),
	action_turn_right.

strategy :-
	next_position(X,Y),
	not(is_wall(X,Y)),
	not(is_border(X,Y)),
	move_forward.

% ----------------------------------- %

