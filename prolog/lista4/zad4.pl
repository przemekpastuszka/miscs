mirror(leaf, leaf).
mirror(node(X, Y, Z), node(Q, Y, P)) :- mirror(X, P), mirror(Z, Q).

flatten(leaf, []).
flatten(node(X, Y, Z), R) :- flatten(X, P), flatten(Z, Q), append(P, [Y|Q], R).




flatten2(A, B) :- flatten_tmp(A, R, []), reverse(R, B). 

flatten_tmp(leaf, X, X).
flatten_tmp(node(X, Y, Z), R, ACC) :- flatten_tmp(X, R1, ACC), flatten_tmp(Z, R, [Y|R1]).  

node(node(leaf,2,node(leaf,4,leaf)), 1, node(node(leaf,5,leaf),3,node(leaf,6,leaf))).
