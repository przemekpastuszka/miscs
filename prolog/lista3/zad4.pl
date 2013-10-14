insert([], E, [E]).
insert([H|T], E, [E,H|T]) :- E < H.
insert([H|T], E, [H|Q]) :- E >= H, insert(T, E, Q). 

ins_sort([], []).
ins_sort([H|T], R) :- ins_sort(T, Q), insert(Q, H, R).
