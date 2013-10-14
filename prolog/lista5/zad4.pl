merge(X, Y, R) :- merge(X, Y, [], A, R1), reverse(A, Q), append(Q, R1, R).

merge(X, [], ACC, ACC, X) :- !.
merge([], Y, ACC, ACC, Y).
merge([H|T], [Q|S], ACC, R1, R2) :- H < Q, merge(T, [Q|S], [H|ACC], R1, R2).
merge([H|T], [Q|S], ACC, R1, R2) :- H >= Q, merge([H|T], S, [Q|ACC], R1, R2).

merge_sort([], []) :- !.
merge_sort([X], [X]) :- !.
merge_sort(X, R) :- halve(X, P, Q), merge_sort(P, R1), merge_sort(Q, R2), merge(R1, R2, R).


