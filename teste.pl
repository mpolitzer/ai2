:- begin_tests(test).

test(consult_antidotes_1) :- consult_antidotes(N), assertion(N==1).
test(consult_ammo_30) :- consult_ammo(N), assertion(N==30).
test(consult_bites_0) :- consult_bites(N), assertion(N==0).
test(consult_position_3_3) :- consult_position(X,Y), assertion(X==3), assertion(Y==3).
test(consult_direction_e) :- consult_direction(N), assertion(N==e).
test(consult_goal_18_17) :- consult_goal(X,Y), assertion(X==18), assertion(Y==17).

test(action_move_forward) :- action_move_forward.

test(sense_zombies_3) :- sense_zombies(N), assertion(N==3).
test(sense_station_0,fail) :- sense_station.
test(sense_hospital_1) :- sense_hospital.
test(sense_antidote_0) :- sense_antidote(N), assertion(N==0).
test(sense_ammo_0) :- sense_ammo(N), assertion(N==0).
test(sense_hit_0,fail) :- sense_hit.

test(action_turn_north) :- action_turn_right, action_turn_right, action_turn_right.
test(action_shoot) :- action_shoot.
test(sense_hit_1) :- sense_hit.
test(sense_zombies_2) :- sense_zombies(N), assertion(N==2).
test(action_move_forward) :- action_move_forward.
test(consult_bites_2) :- consult_bites(N), assertion(N==2).
test(consult_ammo_29) :- consult_ammo(N), asssertion(N==29).
test(action_use_antidote) :- action_use_antidote.
test(consult_antidotes_0) :- consult_antidotes(N), assertion(N==0).
test(consult_bites_0) :- consult_bites(N), assertion(N==0).

test(action_turn_south) :- action_turn_right, action_turn_right.
test(action_move_forward) :- action_move_forward, action_move_forward.
test(sense_station_1) :- sense_station.
test(sense_hospital_1) :- sense_hospital.
test(sense_antidote_1) :- sense_antidote(N), assertion(N==1).
test(action_grab) :- action_grab.
test(consult_antidotes_1) :- consult_antidotes(N), assertion(N==1).

test(action_move_forward) :- action_move_forward.
test(sense_hospital_0,fail) :- sense_hospital.
test(sense_station_1) :- sense_station.
test(sense_ammo_2) :- sense_ammo(N), assertion(N==2).
test(action_grab) :- action_grab.
test(consult_ammo_31) :- consult_ammo(N), assertion(N==31).

test(action_shoot) :- action_shoot.
test(sense_hit_0,fail) :- sense_hit.

:- end_tests(test).
