%top, bottom, left, right neighbourhood is checked
neighbour((X,Y), cell((U,V),_,_,_)) :-
  DX is abs(X - U),
  DY is abs(Y - V),
  1 is DX + DY.

%tie cell with its neighbours
tie(Board, cell(XY, Nghbs, _, _)) :-
	include(neighbour(XY), Board, Nghbs).

%creates board with given width and height. Also it fills cells with init data
createBoard(Width, Height, InitData, Board) :-
	findall(cell((X,Y),_,_,_),
		(between(1, Width, X),
		between(1, Height, Y)),
		Board),
	maplist(tie(Board), Board),
	maplist(init(InitData), Board).

init(InitData, cell((X, Y), _, NR, W)) :-
	member((X, Y, Q), InitData),
	NR is Q - 1,
	W is X * Y,  !.
init(_, _).

%ok, let's play God and create a lake
%first arg: cells which are candidates to become the part of the lake
%second: nr of cells to be add before the lake is finished
%third: nr of the lake which is being created
iWannaLake(X, 0, LakeNr) :- !, 
	coverWithLeeve(X, LakeNr).
iWannaLake([cell(_, Nghbs, _, X) | T], HowMany, LakeNr) :-
	var(X),
	X = LakeNr,
	NHowMany is HowMany - 1,
	append(Nghbs, T, MoreNghbs),
	iWannaLake(MoreNghbs, NHowMany, LakeNr).
iWannaLake([cell(_, Nghbs, _, leeve)|T], HowMany, LakeNr) :-
	!, 
	isNotIsolated(Nghbs),
	iWannaLake(T, HowMany, LakeNr).
iWannaLake([cell(_, _, _, LakeNr)|T], HowMany, LakeNr) :-
	iWannaLake(T, HowMany, LakeNr).

coverWithLeeve([], _) :- !.
coverWithLeeve([cell(_, Nghbs, _, leeve)|T], LakeNr) :-
	!, 
	isNotIsolated(Nghbs),
	coverWithLeeve(T, LakeNr).
coverWithLeeve([cell(_, _, _, LakeNr)|T], LakeNr) :-
	coverWithLeeve(T, LakeNr).

isNotIsolated([cell(_, _, _, X)|_]) :- var(X), !.
isNotIsolated([cell(_, _, _, leeve)|_]) :- !.
isNotIsolated([_|T]) :- isNotIsolated(T).


dfs([], FullBoard, LeeveSize) :-
	!,
	findLeeve(FullBoard, Leeve),
	isLeeveOK([Leeve], LeeveSize).
dfs([cell(_, Nghbs, HowMany, Nr)|Tail], FullBoard, LeeveSize) :-
   iWannaLake(Nghbs, HowMany, Nr),
   dfs(Tail, FullBoard, LeeveSize).

formatSolution([], []).
%formatSolution([cell(_,_,_, X)|T], ['*'|R]) :- var(X), !, formatSolution(T, R).
formatSolution([cell(_,_,_, leeve)|T], ['*'|R]) :- !, formatSolution(T, R).
formatSolution([cell(_,_,Y,_)|T], ['_'|R]) :- var(Y), formatSolution(T, R).
formatSolution([cell(_,_,Y,_)|T], [Q|R]) :- nonvar(Y), Q is Y + 1, formatSolution(T, R).


rowToASCII(Tail, Tail, 0) :- nl.
rowToASCII([H|T], Tail, N) :-
	N > 0,
	write(H),
	write(' '),
	NN is N - 1,
	rowToASCII(T, Tail, NN).

writeBoard(_, _, 0).
writeBoard(Board, Width, N) :-
	N > 0,
	rowToASCII(Board, Tail, Width),
	NN is N - 1,
	writeBoard(Tail, Width, NN).

isInitCell(cell(_,_,A,_)) :- nonvar(A).

main(File) :-
   open(File, read, _, [alias(params)]),
   read(params, Width),
   read(params, Height),
   read(params, InitData),
   close(params),
   createBoard(Width, Height, InitData, Board),
   sum(InitData, Sum),
   LeeveSize is Width * Height - Sum,
   include(isInitCell, Board, LakeInitCells),
   dfs(LakeInitCells, Board, LeeveSize),
   formatSolution(Board, Solution),
   writeBoard(Solution, Width, Height),
   nl.

sum(X, Y) :- sum(X, 0, Y).
sum([], X, X).
sum([(_,_,H)|T], ACC, R) :- NACC is ACC + H, sum(T, NACC, R).
 
findLeeve([Leeve|_], Leeve) :-
	Leeve = cell(_,_,_, leeve), !.
findLeeve([_|T], Leeve) :- findLeeve(T, Leeve).

isLeeveOK(_, 0) :- !.
isLeeveOK([], _) :-  !, fail.
isLeeveOK([cell(_,Nghbs,X,leeve)|T], N) :-
	var(X),
	!,
	X = leeve,
	NN is N - 1,
	append(Nghbs, T, MoreNghbs),
	isLeeveOK(MoreNghbs, NN).
isLeeveOK([_|T], N) :- isLeeveOK(T, N).
	
