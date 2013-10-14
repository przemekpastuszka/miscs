select(H, [H|T], T).
select(X, [H|T], [H|S]) :-
	select(X, T, S).

perm([], []).
perm(L, [X|Y]) :- select(X, L, Q), perm(Q, Y).
