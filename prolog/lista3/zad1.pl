filter([], []).
filter([H|T], [H|L]) :- H >= 0, filter(T, L).
filter([H|T], L) :- H < 0, filter(T, L).

count(_, [], 0).
count(E, [E|T], N) :- count(E, T, Q), N is Q + 1, !.
count(E, [_|T], N) :- count(E, T, N).

exp(_, 0, 1).
exp(B, E, R) :- E > 0, NE is E - 1, exp(B, NE, NR), R is B*NR.
