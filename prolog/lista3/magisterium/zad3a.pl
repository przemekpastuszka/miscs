con_bad(X, Y) :- dir(X, Y).
con_bad(X, Y) :- dir(X, Z),  con_bad(Z, Y).

con_good(X, Y) :- dir(X, Y).
con_good(X, Y) :- con_good(Z, Y), dir(X, Z).

dir(a, b).
dir(b, c).
dir(c, b).
dir(a, d).
dir(d, e).

perm_bad([], []).
perm_bad([H|T], W) :- select(H, W, Y), perm_bad(T, Y).

perm_good([], []).
perm_good([H|T], W) :- perm_good(T, Y), select(H, W, Y).

