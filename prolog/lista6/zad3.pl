sput(E, S, [E|S]).
sget([E|S], E, S).
sempty([]).
saddall(E, G, S, R) :-
	findall(E, G, Q),
	append(Q, S, R).

qput(E, X-[E|Y], X-Y).
qget([E|X]-Y, E, X-Y).
qempty(X) :-
	var(X),
	!,
	X = Y - Y.
qempty(X-Y) :- X == Y.
qaddall(E, G, A-B, A-D) :-
	findall(E, G, B, D).
	
