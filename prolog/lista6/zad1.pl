/*sum(X, Y, Z) :-
	nonvar(X),
	nonvar(Y),
	!,
	Z is X + Y. 

sum(X, Y, Z) :-
	length(C, Z),
	append(A, B, C),
	length(A, X),
	length(B, Y).*/
makeSignForMe(X, X).
makeSignForMe(X, Y) :-
	X \= 0,
	Y is -X.
generate(Y) :-
	nonvar(Y), !.
generate(Y) :-
	length(_, Q),
	makeSignForMe(Q, Y).

sum(X, Y, Z) :- nonvar(X), nonvar(Y), !, Z is X + Y.
sum(X, Y, Z) :- var(X), nonvar(Y), !, generate(Z), X is Z - Y.
sum(X, Y, Z) :- var(Y), nonvar(X), !, generate(Z), Y is Z - X.
sum(X, Y, Z) :- generate(X), generate(Y), Z is X + Y.

