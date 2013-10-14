insert(leaf, E, tree(leaf, E, leaf)).
insert(tree(Left, Key, Right), E, tree(NLeft, Key, Right)) :-
	E < Key,
	insert(Left, E, NLeft).
insert(tree(Left, Key, Right), E, tree(Left, Key, NRight)) :-
	E > Key,
	insert(Right, E, NRight).
	
find(E, tree(_, E, _)).
find(E, tree(Left, Key, _)) :-
	E < Key,
	find(E, Left).
find(E, tree(_, Key, Right)) :-
	E > Key,
	find(E, Right).
	
findMax(tree(_, E, leaf), E).
findMax(tree(_, _, Right), E) :-
	findMax(Right, E).
	
delMax(tree(Left, E, leaf), E, Left).
delMax(tree(L, K, Right), E, tree(L, K, NR)) :-
	delMax(Right, E, NR).

delete(tree(leaf, K, Right), K, Right) :- !.
delete(tree(Left, K, leaf), K, Left) :- !.
delete(tree(Left, K, Right), K, NTree) :-
	przepnij(Left, Right, NTree), !.
delete(tree(_, E, Right), K, NTree) :-
	K > E, !,
	delete(Right, K, NTree).
delete(tree(Left, _, _), K, NTree) :-
	delete(Left, K, NTree). 	

przepnij
	
empty(leaf).
