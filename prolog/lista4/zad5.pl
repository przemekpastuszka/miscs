insert(leaf, X, node(leaf, X, leaf)).
insert(node(X, Y, Z), E, node(R, Y, Z)) :- E < Y, insert(X, E, R).
insert(node(X, Y, Z), E, node(X, Y, R)) :- E >= Y, insert(Z, E, R).

flatten(leaf, []).
flatten(node(X, Y, Z), R) :- flatten(X, P), flatten(Z, Q), append(P, [Y|Q], R).

treesort(L, R) :- treesort(L, R, leaf).
treesort([], R, T) :- flatten(T, R).
treesort([H|L], R, T) :- insert(T, H, NT), treesort(L, R, NT). 
