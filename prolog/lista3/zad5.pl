reverse([], []).
reverse([H|T], R) :- reverse(T, RT), append(RT, [H], R).
