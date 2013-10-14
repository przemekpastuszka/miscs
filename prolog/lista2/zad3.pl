append([], X, X).
append([H|T], X, [H|Y]) :-
   append(T, X, Y).

head(H, [H|_]).
last(L, H) :- append(_, [H], L).
tail(T, [_|T]).
init(L, T) :- append(T, [_], L).

prefix([], _). 
prefix([H|P], [H|L]) :- prefix(P, L).

reverse([], []).
reverse([H|T], X) :- reverse(T, Y), append(Y, [H], X).

sufix(L, S) :- reverse(L, RL), reverse(S, RS), prefix(RS, RL).

