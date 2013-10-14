euclid(M, M, M) :- !.
euclid(M, N, R) :-
	M > N,
	!,
	NewM is M - N,
	euclid(NewM, N, R).
euclid(M, N, R) :-
	NewN is N - M,
	euclid(M, NewN, R).
