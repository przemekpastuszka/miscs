sibling(X, Y) :- parent(Z, X), parent(Z, Y).

sister(X, Y) :- sibling(X, Y), female(X).
grandson(X, Y) :- parent(Y, Z), parent(Z, X), male(X).
cousin(X, Y) :- grandson(X, Z), grandson(Y, Z).
is_mother(X) :- female(X), parent(X, Y).
is_father(X) :- male(X), parent(X, Y).
descendant(X, Y) :- parent(Y, X).
descendant(X, Y) :- parent(Z, X), descendant(Z, Y).

male(adam).
male(john).
male(mark).
male(joshua).
male(david).
female(eve).
female(helen).
female(ivonne).
female(anna).


parent(adam, helen).
parent(adam, ivonne).
parent(adam, anna).
parent(eve, helen).
parent(eve, ivonne).
parent(eve, anna).
parent(john, joshua).
parent(helen, joshua).
parent(ivonne, david).
parent(mark, david).
