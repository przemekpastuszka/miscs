/*
Pracownia nr 2 - zadanie 2: Funol
Przemysław Pastuszka
2010
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

generateTokens([X | T]) --> whitespace, token(X), !, generateTokens(T).
generateTokens([]) --> whitespace, !.
generateTokens(_) --> {write('Error: invalid token'), fail}.

/****************************************************************************************/
%PARSER

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
%INTERPRETER

get([], _, _) :- write('Error: usage of undeclared identifier'), !, fail.
get([(Key, Value)|_], Key, Value) :- !.
get([_|T], K, R) :- get(T, K, R). 

ifEval(bool(true), Fst, _, Env,  R) :- !, evaluate(Fst, Env,  R).
ifEval(bool(false), _, Snd, Env,  R) :- !, evaluate(Snd, Env,  R).
ifEval(_, _, _, _, _) :- write('Error: type boolean expected'), fail.

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
evalBin(_, _, _, _) :- write('Error: wrong usage of system binary function'), fail.

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

fix(lambda([102], appl(lambda([120], lambda([121], appl(appl(id([102]), appl(id([120]), id([120]))), id([121])))), lambda([120], lambda([121], appl(appl(id([102]), appl(id([120]), id([120]))), id([121]))))))).

evaluate(seq(fun(Name, lambda(A, B)), Rest), Env, R) :-
	!,
	fix(X),
	evaluate(appl(X, lambda(Name, lambda(A, B))), Env,  NBody),
	evaluate(Rest, [(Name, NBody) | Env], R).
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
evaluate(list(X), _, list(X)) :- !.
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
evaluate(appl(lambda(V, B), X), Env, R) :- !,
	evaluate(X, Env, XR),
	substitute(B, V, XR, BR),
	evaluate(BR, Env,  R).
evaluate(appl(_, _), _, _) :- !,
	write('Error: wrong application'), fail.
evaluate(_, _, _) :-
	write('Error: given expression cannot be evaluated'), fail.

substitute(num(X), _, _, num(X)) :- !.
substitute(list(X), _, _, list(X)) :- !.
substitute(bool(X), _, _, bool(X)) :- !.
substitute(id(X), X, Y, Y) :- !.
substitute(id(X), _, _, id(X)) :- !.
substitute(fun(X, B), X, _, fun(X, B)) :- !.
substitute(fun(A, B), X, Y, fun(A, R)) :- !,
	substitute(B, X, Y, R).
substitute(lambda(X, B), X, _, lambda(X, B)) :- !.
substitute(lambda(A, B), V, Y, lambda(A, R)) :- !, substitute(B, V, Y, R).
substitute(appl(A, B), X, Y, appl(R1, R2)) :- !,
	substitute(A, X, Y, R1),
	substitute(B, X, Y, R2).
substitute(binary(A, B, C), X, Y, binary(R1, B, R2)) :- !,
	substitute(A, X, Y, R1),
	substitute(C, X, Y, R2).
substitute(prefix(X), _, _, prefix(X)) :- !.
substitute(seq(A, B), X, Y, seq(R1, R2)) :- !,
	substitute(A, X, Y, R1),
	substitute(B, X, Y, R2).
substitute(con(A, B, C), X, Y, con(R1, R2, R3)) :-
	substitute(A, X, Y, R1),
	substitute(B, X, Y, R2),
	substitute(C, X, Y, R3).

/****************************************************************************************/
%WYJŚCIE

clear(num(X), X) :- !.
clear(list([]), []) :- !.
clear(list([H|T]), [A|B]) :- !,
	clear(H, A),
	clear(list(T), B).
clear(bool(X), X) :- !.
clear(lambda(_, _), '<function>') :- !.
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
	evaluate(Y, [], W),
	clear(W, Q),
	write(Q),
	nl.
