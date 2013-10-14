revall(X, Y) :- revall(X, [], Y).

revall([], X, X).
revall([H|T], ACC, R) :- not(is_list(H)), revall(T, [H|ACC], R).
revall([H|T], ACC, R) :- is_list(H), revall(H, Q), revall(T, [Q|ACC], R).
