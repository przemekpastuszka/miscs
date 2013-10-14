split([], _, [], []).
split([H|T], E, [H|S], B) :- H < E, split(T, E, S, B).
split([H|T], E, S, [H|B]) :- H >= E, split(T, E, S, B).

qsort(X, Y) :- qsort(X, [], R), reverse(R, Y).

qsort([], X, X).
qsort([H], X, [H|X]) :- !.
qsort([H|T], ACC, R) :- split(T, H, S, B), qsort(S, ACC, R1), qsort(B, [H|R1], R).
