put(E, X-[E|Y], X-Y).
get([E|X]-Y, E, X-Y).
empty(X) :-
	var(X),
	!,
	X = Y - Y.
empty(X-Y) :- X == Y.
addall(E, G, A-B, A-D) :-
	findall(E, G, B, D).

doThis(Pending, _, _) :-
	empty(Pending),
	!,
	fail.
doThis(Pending, Target, [Target|Result]) :-
	get(Pending, vert(Target, Result), _).
doThis(Pending, Target, Result) :-
	get(Pending, vert(A, Path), Tail),
	A \= Target,
	not(member(A, Path)),
	!,
	addall(vert(Q, [A|Path]), e(A, Q), Tail, NPending),
	doThis(NPending, Target, Result).
doThis(Pending, Target, Result) :-
	get(Pending, _, Tail),
	doThis(Tail, Target, Result).
	
findPath(X, Y, Result) :-
	empty(L),
	put(vert(X, []), L, Pending),
	doThis(Pending, Y, R),
	reverse(R, Result).
	
e(1, 4).
e(1, 2).
e(2, 3).
e(2, 4).
e(3, 4).
e(4, 2).
e(4, 5).
