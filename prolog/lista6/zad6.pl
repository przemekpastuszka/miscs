fill([], []).
fill([c|T], [X|R]) :-
	select(X, [1, 3, 5, 7, 9], _),
	fill(T, R).
fill([s|T], [X|R]) :-
	select(X, [0, 2, 4, 6, 8], _),
	fill(T, R).
	
concat_number(X, Y) :- concat_number(X, 0, Y).
concat_number([], R, R).
concat_number([H|T], ACC, R) :- 
	NACC is ACC * 10 + H, 
	concat_number(T, NACC, R).

decimal(0, [0]).
decimal(N, R) :- 
	N > 0, 
	Q is N // 10, 
	temp(Q, T), 
	H is mod(N, 10),
	append(T, [H], R).
temp(N, R) :- N > 0, decimal(N, R).
temp(N, []) :- N = 0. 
	
solve([A, B|T], [Q, X|R]) :-
	fill(A, Q),
	fill(B, X),
	reverse(X, Y),
	concat_number(X, P),
	concat_number(Q, Z),
	check(Z, P, Y, T, R).

check(Z, X, [], [W], [R]) :-
	P is Z * X,
	decimal(P, R),
	fill(W, R).
check(Z, X, [H|T], [A|B], [N|R]) :-
	P is Z * H,
	decimal(P, N),
	fill(A, N),
	check(Z, X, T, B, R).

	


