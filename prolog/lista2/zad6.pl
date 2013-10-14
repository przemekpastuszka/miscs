/*select(H, [H|T], T).
select(X, [H|T], [H|S]) :-
	select(X, T, S).*/
	
perm([], []).
perm([H|T], W) :- perm(T, Y), select(H, W, Y).	  
