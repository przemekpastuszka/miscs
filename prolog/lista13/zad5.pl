generujPozycje(N, X) :-
	generujPozycje(N, 1, X).
generujPozycje(_, X, X).
generujPozycje(N, L, X) :-
	L < N,
	NL is L + 1,
	generujPozycje(N, NL, X).
	
ustawHetmana(N, Nr, Ls, [(Nr, X)|Ls]) :-
	generujPozycje(N, X),
	sprawdz(Ls, (Nr, X)).
	
sprawdz([], _).
sprawdz([(A, B) | T], (Nr, X)) :-
	B \= X,
	abs(B - X, V),
	abs(A - Nr, H),
	V \= H,
	sprawdz(T, (Nr, X)).
	
rozwiaz(0, _, []) :- !.
rozwiaz(Nr, N, R) :-
	NNr is Nr - 1,
	rozwiaz(NNr, N, Q),
	ustawHetmana(N, Nr, Q, R).
	
