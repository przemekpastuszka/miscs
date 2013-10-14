halve(X, Y, R) :- halve(X, X, [], R1, R), reverse(R1, Y).

halve([], X, Y, Y, X).
halve([_], X, Y, Y, X).
halve([_,_|Q], [H|T], ACC, R1, R2) :- halve(Q, T, [H|ACC], R1, R2).
