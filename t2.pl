
% ------------------------- %

:- dynamic contador/2, direcao_corrente/1, posicao_corrente/1.

% ------------------------- %

tamanho_mapa(20,20).

fora_do_mapa(X,Y) :-
	X < 0;
	Y < 0;
	(
		tamanho_mapa(Xmax, Ymax),
		(
			X >= Xmax;
			Y >= Ymax
		)
	).

% ------------------------- %

posicao_inicial(3,3).
posicao_final(18,17).

posicao_corrente(3,3).

% ------------------------- %

contador(hospitais, 4).
contador(delegacias, 4).
contador(municaos, 30).
contador(antidotos, 1).
contador(pontos, 0).
contador(mordidas, 0).

% ------------------------- %

atualiza_contador(Nome, Incremento) :- 	
	retract(contador(Nome, VelhoValor)), 
	NovoValor is VelhoValor+Incremento, 
	assert(contador(Nome, NovoValor)).

incrementa_contador(Contador) :- atualiza_contador(Contador, 1).
decrementa_contador(Contador) :- atualiza_contador(Contador, -1).

% ------------------------- %

valor_acao(andar, -1).
valor_acao(virar_direcao, -1).
valor_acao(pegar_objeto, -1).
valor_acao(atirar, -10).
valor_acao(ligar_helicoptero, -1).
valor_acao(ser_mordido, -100).
valor_acao(matar_zumbi, 20).
valor_acao(escapar_por_antidoto, 200).

% ------------------------- %

atualiza_pontos(Acao) :-
	valor_acao(Acao, Valor),
	contador(mordidas, QtdMordidas),
	NovoValor is (QtdMordidas+1)*Valor,
	atualiza_contador(pontos, NovoValor).

% ------------------------- %

vetor_direcao(norte, vetor(0, -1)).
vetor_direcao(leste, vetor(1, 0)).
vetor_direcao(sul, vetor(0, 1)).
vetor_direcao(oeste, vetor(-1, 0)).

direcao_corrente(leste).

% ------------------------- %

acao_girar_direita :-
	retract(direcao_corrente(leste)),
	assert(direcao_corrente(sul)),
	atualiza_pontos(virar_direcao).
acao_girar_direita :-
	retract(direcao_corrente(sul)),
	assert(direcao_corrente(oeste)),
	atualiza_pontos(virar_direcao).
acao_girar_direita :-
	retract(direcao_corrente(oeste)),
	assert(direcao_corrente(norte)),
	atualiza_pontos(virar_direcao).
acao_girar_direita :-
	retract(direcao_corrente(norte)),
	assert(direcao_corrente(leste)),
	atualiza_pontos(virar_direcao).

% ------------------------- %

apontar_para(Direcao) :-
	direcao_corrente(X), 
	X = Direcao.
apontar_para(Direcao) :-
	acao_girar_direita,
	apontar_para(Direcao).

% ------------------------- %

posicao_frente(X,Y) :-
	direcao_corrente(Direcao),
	vetor_direcao(Direcao, vetor(dX, dY)),
	posicao_corrente(VelhoX,VelhoY),
	X is VelhoX+dX,
	Y is VelhoY+dY.

pode_andar(X,Y) :-
	posicao_frente(X,Y),
	not(
		parede(X,Y);
		fora_do_mapa(X,Y)
	).

acao_andar :- 
	pode_andar(X,Y),
	retract(posicao_corrente(_,_)),
	assert(posicao_corrente(X,Y)),
	atualiza_pontos(andar).

% ------------------------- %



% ------------------------- %

