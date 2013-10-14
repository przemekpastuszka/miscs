direct(wroclaw, wawa).
direct(wroclaw, krak).
direct(wroclaw, szczecin).
direct(szczecin, lublin).
direct(szczecin, gniezno).
direct(wawa, katowice).
direct(gniezno, gliwice).
direct(lublin, gliwice).

connection(X, Y) :- direct(X, Y).
connection(X, Y) :- direct(X, Z), connection(Z, Y).
