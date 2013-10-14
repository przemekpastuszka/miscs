empty(X-Y) :- X == Y.
get([H|T]-Y, H, T-Y).
add(El, X-[El|T], X-T).

toList(X, []) :- empty(X), !.
toList(X, [H|T]) :- get(X, H, Q), toList(Q, T).

merge(X, Y, R) :- merge(X, Y, [], A, R1), reverse(A, Q), append(Q, R1, R).
merge(X, [], ACC, ACC, X) :- !.
merge([], Y, ACC, ACC, Y).
merge([H|T], [Q|S], ACC, R1, R2) :- H < Q, merge(T, [Q|S], [H|ACC], R1, R2).
merge([H|T], [Q|S], ACC, R1, R2) :- H >= Q, merge([H|T], S, [Q|ACC], R1, R2).

halve(X-Y, X-A, A-Y) :- halve2(X-Y, X-Y, A).
halve2(L-T, A-_, A) :- empty(L-T), !.
halve2(L-T, A-_, A) :- get(L-T, _, Q), empty(Q), !.
halve2(X, Y, R) :- get(X, _, Q), get(Q, _, P), get(Y, _, N), halve2(P, N, R).

merge_sort(X, []) :- empty(X), !.
merge_sort(X, [H]) :- get(X, H, Q), empty(Q), !.
merge_sort(X, R) :- halve(X, P, Q), merge_sort(P, R1), merge_sort(Q, R2), merge(R1, R2, R).
