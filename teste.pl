:- begin_tests(test).

test(consult_antidotes_1) :- consult_antidotes(N).
test(consult_ammo_30) :- consult_ammo(N).
test(consult_bites_0) :- consult_bites(N).
test(consult_position_3-3) :- consult_position(X,Y).
test(consult_direction_e) :- consult_direction(N).
test(consult_goal_18-17) :- consult_goal(X,Y).

test(action_move_forward) :- action_move_forward.

test(sense_zombies_3) :- sense_zombies(N).
test(sense_station_0) :- sense_station.
test(sense_hospital_1) :- sense_hospital.
test(sense_antidote_0) :- sense_antidote(N).
test(sense_ammo_0) :- sense_ammo(N).
test(sense_hit_0) :- sense_hit.

test(action_turn_north) :- action_turn_right, action_turn_right, action_turn_right.
test(action_shoot) :- action_shoot.
test(sense_hit_1) :- sense_hit.
test(sense_zombies_2) :- sense_zombies(N).
test(action_move_forward) :- action_move_forward.
test(consult_bites_2) :- consult_bites(N).
test(consult_ammo_29) :- consult_ammo(N).
test(action_use_antidote) :- action_use_antidote.
test(consult_antidotes_0) :- consult_antidotes(N).
test(consult_bites_0) :- consult_bites(N).

test(action_turn_south) :- action_turn_right, action_turn_right.
test(action_move_forward) :- action_move_forward, action_move_forward.
test(sense_station_1) :- sense_station.
test(sense_hospital_1) :- sense_hospital.
test(sense_antidote_1) :- sense_antidote(N).
test(action_grab) :- action_grab.
test(consult_antidotes_1) :- consult_antidotes(N).

test(action_move_forward) :- action_move_forward.
test(sense_hospital_0) :- sense_hospital.
test(sense_station_1) :- sense_station.
test(sense_ammo_1) :- sense_ammo(N).
test(action_grab) :- action_grab.
test(consult_ammo_31) :- consult_ammo(N).

test(action_shoot) :- action_shoot.
test(sense_hit_0) :- sense_hit.

:- end_tests(test).
