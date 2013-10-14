/*
Pracownia nr 2 - zadanie 2: Funol
Przemysław Pastuszka
2010

Program działa kilkuetapowo:
-wczytanie zawartości pliku wejściowego
-generacja listy tokenów
-generacja drzewa rozbioru
-preprocesing
-ewaluacja
-formatowanie wyjścia
*/

/****************************************************************************************/
%LEXER
comment --> [A], {A \= 10}, !, comment. %wycięcie komentarzy
comment --> "".

%pozbycie się białych znaków (komentarz również traktowany jako biały znak)
whitespace --> "--", !, comment, whitespace. 
whitespace --> " ", !, whitespace.
whitespace --> "\t", !, whitespace.
whitespace --> "\n", !, whitespace.
whitespace --> "".

digit(X) --> [A], {A >= 48, A < 58, X is A - 48}. %cyfra
intiger(X) --> digit(A), intiger(X, A). %literał całkowitoliczbowy
intiger(X, A) --> digit(Q), !, {NA is A * 10 + Q}, intiger(X, NA).
intiger(X, X) --> "". 

letter(X) --> [X], {X >= 65, X < 91; X >= 97, X <123}. %litera
ident([X|T]) --> letter(X), ident2(T). %identyfikator
ident2([X|T]) --> digit(X), !, ident2(T).
ident2([X|T]) --> letter(X), !, ident2(T).
ident2([95|T]) --> "_", !, ident2(T).
ident2([39|T]) --> "'", !, ident2(T).
ident2([]) --> "".

%wszystkie możliwe tokeny
token(let) --> "let", !.
token(if) --> "if", !.
token(then) --> "then", !.
token(else) --> "else", !.
token(bslash) --> "\\", !.
token(arrow) --> "->", !.
token(lpar) --> "(", !.
token(rpar) --> ")", !.
token(larr) --> "[", !.
token(rarr) --> "]", !.
token(coma) --> ",", !.
token(semicolon) --> ";", !.
token(lesseq) --> "<=", !.
token(greq) --> ">=", !.
token(less) --> "<", !.
token(gr) --> ">", !.
token(and) --> "and", !.
token(or) --> "or", !.
token(not) --> "not", !.
token(neq) --> "/=", !.
token(eq) --> "=", !.
token(plplus) --> "++", !.
token(plus) --> "+", !.
token(minus) --> "-", !.
token(mul) --> "*", !.
token(divT) --> "div", !.
token(modT) --> "mod", !.
token(colon) --> ":", !.
token(null) --> "null", !.
token(head) --> "head", !.
token(tail) --> "tail", !.
token(id(X)) --> ident(X), !.
token(num(X)) --> intiger(X).

%zamiana wejściowego łańcucha znaków na listę tokenów
generateTokens([X | T]) --> whitespace, token(X), !, generateTokens(T).
generateTokens([]) --> whitespace, !.
generateTokens(_) --> {write('Error: invalid token'), fail}.

/****************************************************************************************/
%PARSER


%w parserze nie dzieje się nic ciekawego
%to niemal dosłowna implementacja zadanej gramatyki Funola
program(X) --> expr(X), !.
program(_) --> {write('Error: invalid program structure'), fail}.

expr(seq(fun(Name, Body), Rest)) --> 
	[let], !, [id(Name)], [eq], expr(Body), [semicolon], 
	expr(Rest).
expr(con(Condition, Fst, Snd)) --> 
	[if], !, expr(Condition), [then], expr(Fst), [else], expr(Snd).
expr(lambda(Var, Body)) -->
	[bslash], !, [id(Var)], [arrow], expr(Body).
expr(X) --> simpExpr(X).

prefixOp(null) --> [null], !.
prefixOp(head) --> [head], !.
prefixOp(tail) --> [tail], !.
prefixOp(not) --> [not].

binaryOne(mul) --> [mul], !.
binaryOne(divT) --> [divT], !.
binaryOne(modT) --> [modT].

binaryTwo(plus) --> [plus], !.
binaryTwo(minus) --> [minus].

binaryThree(colon) --> [colon], !.
binaryThree(plplus) --> [plplus].

binaryFour(lesseq) --> [lesseq], !.
binaryFour(greq) --> [greq], !.
binaryFour(less) --> [less], !.
binaryFour(gr) --> [gr], !.
binaryFour(neq) --> [neq], !.
binaryFour(eq) --> [eq].

binaryFive(and) --> [and], !.
binaryFive(or) --> [or].

simpExOne(X) --> app(Y), simpExOne(X, Y).
simpExOne(X, Y) --> binaryOne(Z), !, app(Q), simpExOne(X, binary(Y, Z, Q)).
simpExOne(X, X) --> "".

simpExTwo(X) --> simpExOne(Y), simpExTwo(X, Y).
simpExTwo(X, Y) --> binaryTwo(Z), !, simpExOne(Q), simpExTwo(X, binary(Y, Z, Q)).
simpExTwo(X, X) --> "".

simpExThree(binary(X, Y, Z)) --> simpExTwo(X), binaryThree(Y), !, simpExThree(Z).
simpExThree(X) --> simpExTwo(X).

simpExFour(binary(X, Y, Z)) --> simpExThree(X), binaryFour(Y), !, simpExThree(Z).
simpExFour(X) --> simpExThree(X).

simpExpr(X) --> simpExFour(Y), simpExpr(X, Y).
simpExpr(X, Y) --> binaryFive(Z), !, simpExFour(Q), simpExpr(X, binary(Y, Z, Q)).
simpExpr(X, X) --> "".

app(X) --> atomicE(Y), app(X, Y).
app(X, Y) --> atomicE(Z), app(X, appl(Y, Z)).
app(X, X) --> "".

atomicE(X) --> [lpar], !, expr(X), [rpar].
atomicE(prefix(X)) --> prefixOp(X), !.
atomicE(id(X)) --> [id(X)], !.
atomicE(num(X)) --> [num(X)], !.
atomicE(list(X)) --> [larr], expSeq(X), [rarr], !.
atomicE(list([])) --> [larr], [rarr].

expSeq([H|T]) --> expr(H), [coma], !, expSeq(T).
expSeq([X]) --> expr(X).

/****************************************************************************************/
%PREPROCESOR

%w trakcie działania interpretera może zdarzyć się sytuacja, iż zmienna, która była dotąd wolna,
%zostaje związana. Oczywiście, chcielibyśmy takich sytuacji uniknąć, dlatego zmieniamy nazwy
%zmiennych wolnych na ":free variable", co rozwiązuje problem

renameFV(lambda(A, B), Env, lambda(A, X)) :- !,
	renameFV(B, [A|Env], X).
renameFV(num(X), _, num(X)) :- !.
renameFV(list(X), Env, list(Y)) :- !,
	renList(X, Env, Y).
renameFV(bool(X), _, bool(X)) :- !.
renameFV(id(X), Env, id(X)) :- member(X, Env), !.
renameFV(id(_), _, id(":free variable")) :- !.
renameFV(seq(fun(X, A), R), Env, seq(fun(X, Q), L)) :- !,
	renameFV(A, [X|Env], Q),
	renameFV(R, [X|Env], L).
renameFV(appl(A, B), Env, appl(R1, R2)) :- !,
	renameFV(A, Env, R1),
	renameFV(B, Env, R2).
renameFV(binary(A, X, B), Env, binary(R1, X, R2)) :- !,
	renameFV(A, Env, R1),
	renameFV(B, Env, R2).
renameFV(prefix(X), _, prefix(X)) :- !.
renameFV(con(A, B, C), Env, con(R1, R2, R3)) :- !,
	renameFV(A, Env, R1),
	renameFV(B, Env, R2),
	renameFV(C, Env, R3).

renList([], _, []) :- !.
renList([H|T], Env, [A|B]) :-
	renameFV(H, Env, A),
	renList(T, Env, B).

/****************************************************************************************/
%INTERPRETER

%pobranie funkcji zadeklarowanej w środowisku
get([], K, _) :- write('Error: usage of undeclared identifier - '), writef("%s", [K]), !, fail.
get([(Key, Value)|_], Key, Value) :- !.
get([_|T], K, R) :- get(T, K, R). 

%ewaluacja instrukcji if then else
ifEval(bool(true), Fst, _, Env,  R) :- !, evaluate(Fst, Env,  R).
ifEval(bool(false), _, Snd, Env,  R) :- !, evaluate(Snd, Env,  R).
ifEval(_, _, _, _, _) :- write('Error: type boolean expected'), fail.

%ewaluacja funkcji binarnych
evalBin(mul, num(A), num(B), num(R)) :- !, R is A * B.
evalBin(X, num(_), num(0), _) :-
	(X = divT; X = modT),
	write('Error: division by zero'), !, fail. 
evalBin(divT, num(A), num(B), num(R)) :- !, R is A // B.
evalBin(modT, num(A), num(B), num(R)) :- !, R is mod(A, B).
evalBin(plus, num(A), num(B), num(R)) :- !, R is A + B.
evalBin(minus, num(A), num(B), num(R)) :- !, R is A - B.
evalBin(colon, A, list(B), list([A|B])) :- !.
evalBin(plplus, list(A), list(B), list(R)) :- !, append(A, B, R).
evalBin(eq, num(A), num(B), bool(true)) :- A = B, !.
evalBin(eq, num(_), num(_), bool(false)) :- !.
evalBin(neq, num(A), num(B), bool(false)) :- A = B, !.
evalBin(neq, num(_), num(_), bool(true)) :- !.
evalBin(gr, num(A), num(B), bool(true)) :- A > B, !.
evalBin(gr, num(_), num(_), bool(false)) :- !.
evalBin(less, num(A), num(B), bool(true)) :- A < B, !.
evalBin(less, num(_), num(_), bool(false)) :- !.
evalBin(lesseq, num(A), num(B), bool(false)) :- A > B, !.
evalBin(lesseq, num(_), num(_), bool(true)) :- !.
evalBin(greq, num(A), num(B), bool(true)) :- A >= B, !.
evalBin(greq, num(_), num(_), bool(false)) :- !.
evalBin(and, bool(true), bool(true), bool(true)) :- !.
evalBin(and, bool(_), bool(_), bool(false)) :- !.
evalBin(or, bool(false), bool(false), bool(false)) :- !.
evalBin(or, bool(_), bool(_), bool(true)) :- !.
evalBin(X, Y, Z, _) :- 
	write('Error: wrong usage of system binary function: '), 
	write(X), write('\n'), write(Y), write('\n\n'), write(Z), fail.

%ewaluacja funkcji prefixowych
evalPref(null, list([]), bool(true)) :- !.
evalPref(null, list(_), bool(false)) :- !.
evalPref(head, list([H|_]), H) :- !.
evalPref(head, list([]), _) :- !,
	write('Error: cannot get head from empty list'), fail.
evalPref(tail, list([_|T]), list(T)) :- !.
evalPref(tail, list([]), _) :- !,
	write('Error: cannot get tail from empty list'), fail.
evalPref(not, bool(true), bool(false)) :- !.
evalPref(not, bool(false), bool(true)) :- !.
evalPref(_, _, _) :- write('Error: wrong usage of system prefix function'), fail.

%ewaluacja listy
evalList([], _, []) :- !.
evalList([H|T], Env, [A|B]) :-
	evaluate(H, Env, A),
	evalList(T, Env, B).

%w moim programie korzystam ze standardowego kombinatora punktu stałego Y danego wzorem:
%Y = \f -> (\x -> f (x x)) (\x -> f (x x))
%Jednak ze względu na fakt, że Funol jest językiem gorliwym i ewaluacja powyższego wyrażenia
%nie zakończyłaby się nigdy, konieczna była transformacja tego kombinatora do postaci:
%Y = \f -> (\x -> \y -> f (x x) y) (\x -> \y -> f (x x) y)
%Poniżej znajduje się drzewo rozbioru tego właśnie kombinatora
fix(lambda(f, 
	appl(lambda(x, lambda(y, appl(appl(id(f), appl(id(x), id(x))), id(y)))), 
	lambda(x, lambda(y, appl(appl(id(f), appl(id(x), id(x))), id(y))))))).

%chyba najciekawszy punkt ewaluacji, ponieważ to tutaj rekursja zostaje "obsłużona"
%Najpierw funkcja jest domykana ze względu na rekursję, czyli:
%fun(Nazwa, lambda(Arg, Cialo)) staje się fun(Nazwa, lambda(Nazwa, lambda(Arg, Cialo)))
%tak przygotowana funkcja może zostać zaaplikowana do kombinatora punktu stałego
%i rekursja zostaje pięknie obsłużona
evaluate(seq(fun(Name, lambda(A, B)), Rest), Env, R) :-
	!,
	fix(X),
	evaluate(appl(X, lambda(Name, lambda(A, B))), Env,  NBody),
	evaluate(Rest, [(Name, NBody) | Env], R).
%dla funkcji bezparametrowych (stałych) nie używamy kombinatora Y
%(gdyż przystosowanie Y do języka gorliwego zabiło niektóre z jego właściwości)
evaluate(seq(fun(Name, Body), Rest), Env,  R) :-
	!,
	evaluate(Body, Env,  NBody),
	evaluate(Rest, [(Name, NBody) | Env],  R).
evaluate(con(Condition, Fst, Snd), Env, R) :- 
	!, 
	evaluate(Condition, Env, B), 
	ifEval(B, Fst, Snd, Env, R).
evaluate(lambda(V, B), _, lambda(V, B)) :- !.
evaluate(num(X), _, num(X)) :- !.
evaluate(list(X), Env, list(Y)) :- !,
	evalList(X, Env, Y).
evaluate(bool(X), _, bool(X)) :- !.
evaluate(prefix(X), _, prefix(X)) :- !.
evaluate(id(X), Env, R) :- !, get(Env, X, R).
evaluate(binary(X, Y, Z), Env, R) :- !, 
	evaluate(X, Env, XR), 
	evaluate(Z, Env, ZR), 
	evalBin(Y, XR, ZR, R).
evaluate(appl(prefix(X), Y), Env, R) :- !,
	evaluate(Y, Env, YR),
	evalPref(X, YR, R).
evaluate(appl(id(X), Y), Env, R) :- !,
	get(Env, X, XR), evaluate(appl(XR, Y), Env, R).
evaluate(appl(appl(A, B), C), Env, R) :- !,
	evaluate(appl(A, B), Env, ABR),
	evaluate(appl(ABR, C), Env, R).
%aplikacja argumentu do funkcji przebiega trzyetapowo:
%-oblicz wartość argumentu
%-wynik podstaw za zmienną
%-oblicz wartość otrzymanego drzewa
evaluate(appl(lambda(V, B), X), Env, R) :- !,
	evaluate(X, Env, XR),
	substitute(B, V, XR, BR),
	evaluate(BR, Env,  R).
evaluate(appl(A, B), _, _) :- !,
	write('Error: wrong application: '), write(B), write(' applied to '), write(A), fail.
evaluate(Q, _, _) :-
	write('Error: given expression: '), write(Q), write(' cannot be evaluated'), fail.

%zwykłe podstawienie danej wartości za każde wystąpienie zmiennej (identyfikatora)
substitute(num(X), _, _, num(X)) :- !.
substitute(list(X), A, B, list(Y)) :- !,
	subList(X, A, B, Y).
substitute(bool(X), _, _, bool(X)) :- !.
substitute(id(X), X, Y, Y) :- !.
substitute(id(X), _, _, id(X)) :- !.
substitute(seq(fun(X, B), R), X, _, seq(fun(X, B), R)) :- !.
substitute(seq(fun(A, B), R), X, Y, seq(fun(A, L), Q)) :- !,
	substitute(B, X, Y, L),
	substitute(R, X, Y, Q).
substitute(lambda(X, B), X, _, lambda(X, B)) :- !.
substitute(lambda(A, B), V, Y, lambda(A, R)) :- !, substitute(B, V, Y, R).
substitute(appl(A, B), X, Y, appl(R1, R2)) :- !,
	substitute(A, X, Y, R1),
	substitute(B, X, Y, R2).
substitute(binary(A, B, C), X, Y, binary(R1, B, R2)) :- !,
	substitute(A, X, Y, R1),
	substitute(C, X, Y, R2).
substitute(prefix(X), _, _, prefix(X)) :- !.
substitute(con(A, B, C), X, Y, con(R1, R2, R3)) :-
	substitute(A, X, Y, R1),
	substitute(B, X, Y, R2),
	substitute(C, X, Y, R3).

subList([], _, _, []) :- !.
subList([H|T], X, Y, [A|B]) :-
	substitute(H, X, Y, A),
	subList(T, X, Y, B).
/****************************************************************************************/
%WYJŚCIE

%tutaj pozbywamy się nadmiarowych termów, które wcześniej były pomocne w kontroli typów
clear(num(X), X) :- !.
clear(list([]), []) :- !.
clear(list([H|T]), [A|B]) :- !,
	clear(H, A),
	clear(list(T), B).
clear(bool(X), X) :- !.
clear(lambda(_, _), '<function>') :- !.
clear(prefix(_), '<function>') :- !.
clear(_, _) :- write('Error: ooops, something has gone wrong. Sorry'), fail.

/****************************************************************************************/
%WEJŚCIE

fileToString(File, X) :-
	seeing(Old),
	see(File),
	getCharacters(X),
	seen,
	see(Old). 

process(-1, []) :- !.
process(X, [X|S]) :- getCharacters(S).

getCharacters(X) :- get0(Y), process(Y, X).

/****************************************************************************************/
%PROCEDURA MAIN

main(File) :-
	fileToString(File, Program),
	phrase(generateTokens(X), Program),
	phrase(program(Y), X),
	renameFV(Y, [], Z),
	evaluate(Z, [], W),
	clear(W, Q),
	write(Q),
	nl.
	
/****************************************************************************************/
%KONIEC
/****************************************************************************************/
