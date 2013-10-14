sublist(_, []).
sublist([H|L], [H|S]) :- sublist(L, S).
sublist([_|L], S) :- sublist(L, S).


concat_number(X, Y) :- reverse(X, Z), concat_number2(Z, Y).
concat_number2([], 0).
concat_number2([H|T], N) :- concat_number2(T, R), N is R * 10 + H.

/*puzzle(A, C, E, 1, R, S, U) :- solve([A, C, E, R, S, U]).*/
solve(A, C, E, 1, R, S, U) :- 
	sublist([0, 1, 2, 3, 4, 5, 6, 7, 8, 9], X),
	length(X, 6),
	permutation(X, [A, C, E, R, S, U]),
	U \= 0,
	usa([A, C, E, R, S, U], USA),
	ussr([A, C, E, R, S, U], USSR),
	peace([A, C, E, R, S, U], PEACE),
	PEACE is (USA + USSR).


usa([A, _, _, _, S, U], N) :- concat_number([U, S, A], N).
ussr([_, _, _, R, S, U], N) :- concat_number([U, S, S, R], N).
peace([A, C, E, _, _, _], N) :- concat_number([1, E, A, C, E], N).
