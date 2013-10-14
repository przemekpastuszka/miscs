/*flatten(X, Y) :- flatten(X, [], R), reverse(R, Y).

flatten([], X, X).
flatten([H|T], ACC, Y) :- not(is_list(H)), flatten(T, [H|ACC], Y).
flatten([H|T], ACC, Y) :- is_list(H), flatten(H, ACC, R), flatten(T, R, Y).  */

empty(X-X).
get([H|T]-Y, H, T-Y).
add(El, X-[El|T], X-T).

flatten(X, Y) :- empty(L), flatten(X, L, Y-[]).

flatten([], X, X).
flatten([H|T], ACC, Y) :- not(is_list(H)), add(H, ACC, NACC), flatten(T, NACC, Y).
flatten([H|T], ACC, Y) :- is_list(H), flatten(H, ACC, R), flatten(T, R, Y). 
