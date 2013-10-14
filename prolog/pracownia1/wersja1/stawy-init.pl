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
	member((X, Y, NR), InitData), W is X * Y,  !.
init(_, _).

%neighbour is valid when it represents nonlake field or part of lake which is being created at the moment
/*isValidNeighbour(cell(_, _, _, X), _) :- 
	var(X).
isValidNeighbour(cell(_, _, _, X), NR) :- 
	nonvar(X),
	X = NR.

%checks if all neighbours all valid
areNeighboursValid(Nghbs, NR) :-
	findall(X,
	(member(X, Nghbs),
	isValidNeighbour(X, NR)),
	Nghbs).*/

%ok, let's play God and create a lake
%first arg: cells which are candidates to become the part of the lake
%second: nr of cells to be add before the lake is finished
%third: nr of the lake which is being created
/*iWannaLake(_, 0, _).
iWannaLake([cell(_, Nghbs, _, X) | T], HowMany, LakeNr) :-
	HowMany > 0,
	var(X),
	areNeighboursValid(Nghbs, LakeNr),
	X = LakeNr,
	NHowMany is HowMany - 1,
	append(Nghbs, T, MoreNghbs),
	iWannaLake(MoreNghbs, NHowMany, LakeNr).
iWannaLake([_|T], HowMany, LakeNr) :-
	HowMany > 0,
	iWannaLake(T, HowMany, LakeNr).*/
iWannaLake([], 0, _).
iWannaLake([cell(_, Nghbs, _, X) | T], HowMany, LakeNr) :-
	var(X),
	HowMany > 0,
	X = LakeNr,
	NHowMany is HowMany - 1,
	append(Nghbs, T, MoreNghbs),
	iWannaLake(MoreNghbs, NHowMany, LakeNr).
iWannaLake([cell(_, _, _, leeve)|T], HowMany, LakeNr) :-
	!, iWannaLake(T, HowMany, LakeNr).
iWannaLake([cell(_, _, _, LakeNr)|T], HowMany, LakeNr) :-
	iWannaLake(T, HowMany, LakeNr).


nextFree([cell(_,_,A,_) | Board], Cell, Tail) :-
   var(A),
   !,
   nextFree(Board, Cell, Tail).
nextFree([Cell | Board], Cell, Board).

dfs(_, FullBoard, 0, LeeveSize). /* :-
	findLeeve(FullBoard, Leeve),
	isLeeveOK([Leeve], LeeveSize).*/
dfs(Board, FullBoard, NrOfLakes, LeeveSize) :-
   NrOfLakes > 0,
   nextFree(Board, cell(_, Nghbs, HowMany, Nr), Tail),
   NHowMany is HowMany - 1,
   iWannaLake(Nghbs, NHowMany, Nr),
   NNrOfLakes is NrOfLakes - 1,
   dfs(Tail, FullBoard, NNrOfLakes, LeeveSize).

formatSolution([], []).
formatSolution([cell(_,_,_, X)|T], ['*'|R]) :- var(X), !, formatSolution(T, R).
formatSolution([cell(_,_,_, leeve)|T], ['l'|R]) :- !, formatSolution(T, R).
formatSolution([cell(_,_,Y,_)|T], ['_'|R]) :- var(Y), formatSolution(T, R).
formatSolution([cell(_,_,Y,_)|T], [Y|R]) :- nonvar(Y), formatSolution(T, R).


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

main(File) :-
   open(File, read, _, [alias(params)]),
   read(params, Width),
   read(params, Height),
   read(params, InitData),
   close(params),
   createBoard(Width, Height, InitData, Board),
   length(InitData, NrOfLakes),
   sum(InitData, Sum),
   LeeveSize is Width * Height - Sum,
   dfs(Board, Board, NrOfLakes, LeeveSize),
   formatSolution(Board, Solution),
   writeBoard(Solution, Width, Height),
   nl.

sum(X, Y) :- sum(X, 0, Y).
sum([], X, X).
sum([(_,_,H)|T], ACC, R) :- NACC is ACC + H, sum(T, NACC, R).
 
findLeeve([cell(_,_,_,X)|T], Leeve) :- 
	nonvar(X), 
	!, 
	findLeeve(T, Leeve).
findLeeve([Leeve|_], Leeve).

isLeeveOK(_, 0) :- !.
isLeeveOK([], N) :- N > 0, !, fail.
%isLeeveOK([cell(_,_,_,leeve)|_], _) :- !, fail.
isLeeveOK([cell(_,_,_,X)|T], N) :- nonvar(X), isLeeveOK(T, N).
isLeeveOK([cell(_,Nghbs,_,X)|T], N) :-
	var(X),
	X = leeve,
	NN is N - 1,
	append(Nghbs, T, MoreNghbs),
	isLeeveOK(MoreNghbs, NN).
	
