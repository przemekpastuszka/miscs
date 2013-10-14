/*BINARY*/
bin([0]).
bin(X) :- bin(X, []).

bin([1|X], Y) :- generate(X, Y).
bin(X, Y) :- bin(X, [1|Y]).

generate([], []).
generate([0|X], [_|T]) :- generate(X, T).
generate([1|X], [_|T]) :- generate(X, T).

/*REVERSED BINARY*/
rbin([0]).
rbin(X) :- rbin(X, []).

rbin(X, Y) :- inc(Y, X).
rbin(X, Y) :- inc(Y, Z), rbin(X, Z).

inc([], [1]).
inc([0|T], [1|T]).
inc([1|T], [0|S]) :- inc(T, S).

