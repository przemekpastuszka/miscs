
	
[user].
:- arithmetic_function(!/1).

[user].
:- op(100, xf, !).
	
!(0, 1) :- !.
!(N, R) :-
	NN is N - 1,
	!(NN, A),
	R is A * N.
