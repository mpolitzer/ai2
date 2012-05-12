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

f :- action_move_forward.
r :- action_turn_right.
l :-	action_turn_right,
	action_turn_right,
	action_turn_right.
g :- action_grab.
s :- action_shoot.
ua :- action_use_antidote.
on :- action_turn_chopper_on.
sz(N) :- sense_zombies(N).

% ----------------------------------- %

:- dynamic
	has_zombie/3,
	unsafe_tile/2,
	visited/2.

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

turn_to(D) :-
	(consult_direction(OldD),
	not(D = OldD),
	action_turn_right,
	turn_to(D));true.

% ----------------------------------- %

strategy :-
	consult_position(X,Y),
	consult_goal(GX,GY),
	X = GX,
	Y = GY,
	action_turn_chopper_on.

strategy :-
       true.	

% ----------------------------------- %

