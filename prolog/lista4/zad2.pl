member(H, [H|_]).
member(H, [_|T]) :- member(H, T).

trip(X, Y, [X|R]) :- trip(X, Y, R, [X, Y]).
trip(X, Y, [Y], _) :- connection(X, Y).
trip(X, Y, [Z | R], L) :- connection(X, Z), not(member(Z, L)), trip(Z, Y, R, [Z | L]).

connection(gliwice, wroclaw).
connection(wroclaw, durniow).
connection(wroclaw, warszawa).
connection(warszawa, durniow).
connection(wroclaw, gliwice).
connection(warszawa, gliwice).
