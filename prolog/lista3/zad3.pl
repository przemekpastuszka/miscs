/*select_min([], _, []).
select_min([H|T], M, R) :- find_min(T, H, M), select(M, [H|T], R).

find_min([], H, H).
find_min([H|T], M, R) :- H < M, find_min(T, H, R).
find_min([H|T], M, R) :- H >= M, find_min(T, M, R).*/

select_min([], _, []).
select_min([H|T], M, R) :- find_min(T, H, R, M).

find_min([], M, [], M).
find_min([H|T], M, [M|R], R2) :- H < M, find_min(T, H, R, R2).
find_min([H|T], M, [H|R], R2) :- H >= M, find_min(T, M, R, R2).

sel_sort([], []).
sel_sort([H|T], [M|R]) :- select_min([H|T], M, Q), sel_sort(Q, R).
