empty(X-Y) :- X == Y.
put(El, X-[El|Y], X-Y).
get([H|T]-Y, H, T-Y).

check(_, L) :- empty(L), !.
check(N, L) :- get(L, H, T), not((0 is mod(N, H))), check(N, T).
	
nextPrime(L, N, N) :- check(N, L), !.
nextPrime(L, N, R) :-
	Q is N + 1,
	nextPrime(L, Q, R).
	
genPrimes(A, B, A, B).
genPrimes(A, B, R1, R2) :-
	nextPrime(A, B, Q2),
	put(Q2, A, Q1),
	genPrimes(Q1, Q2, R1, R2).
	
prime(X) :- 
	var(X),
	!,
	genPrimes([2|Y]-Y, 2, _, X).

prime(X) :-
	W is X,
	prime(Y),
	(W = Y, !; Y > W, !, fail).


	
	

