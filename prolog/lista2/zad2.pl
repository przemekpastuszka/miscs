even([]).
even([_,_|X]) :- even(X).

append([], X, X).
append([H|T], X, [H|Y]) :-
   append(T, X, Y).


palindrom([]).
palindrom([_]).
palindrom([H|T]) :- append(X,[H],T), palindrom(X).

singleton([_]).
