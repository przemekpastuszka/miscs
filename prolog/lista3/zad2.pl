factorial(0, 1).
factorial(N, M) :- 
	N > 0, 
	Q is N - 1, 
	factorial(Q, NM), 
	M is N * NM.

concat_number([], 0).
concat_number(L, N) :- append(T, [H], L), concat_number(T, R), N is R * 10 + H.

decimal(0, [0]).
decimal(N, R) :- 
	N > 0, 
	Q is N // 10, 
	temp(Q, T), 
	H is mod(N, 10),
	append(T, [H], R).
temp(N, R) :- N > 0, decimal(N, R).
temp(N, []) :- N = 0. 
