sublist(_, []).
sublist([H|L], [H|S]) :- sublist(L, S).
sublist([_|L], S) :- sublist(L, S).
