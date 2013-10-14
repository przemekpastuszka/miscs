%PROCEDURA MAIN

%proste sprawdzenie czy komórka została wypełniona danymi wejściowymi
isInitCell(cell(_,_,A,_)) :- nonvar(A).

main(File) :-
   open(File, read, _, [alias(params)]), %rozpoczęcie odczytu z pliku
   read(params, Width),
   read(params, Height),
   read(params, InitData),
   close(params), %zakończenie odczytu
   createBoard(Width, Height, InitData, Board),
   sum(InitData, Sum), %obliczamy łączną powierzchnię stawów
   LeeveSize is Width * Height - Sum, %obliczamy powierzchnię grobli
   include(isInitCell, Board, LakeInitCells), 
   dfs(LakeInitCells, Board, LeeveSize), %oblicz rozwiązanie
   formatSolution(Board, Solution), %sformatuj do ASCII artu
   writeBoard(Solution, Width, Height), %i wypisz
   nl.

%zwykła suma po danych wejściowych
sum(X, Y) :- sum(X, 0, Y).
sum([], X, X).
sum([(_,_,H)|T], ACC, R) :- NACC is ACC + H, sum(T, NACC, R).


%INICJALIZACJA

%sprawdzenie czy zadana komórka sąsiaduje z komórką o wsp. X, Y
%rozważamy sąsiedztwo w pionie i poziomie
neighbour((X,Y), cell((U,V),_,_,_)) :-
  DX is abs(X - U),
  DY is abs(Y - V),
  1 is DX + DY.

%do każdej komórki dodajemy listę jej sąsiadów
tie(Board, cell(XY, Nghbs, _, _)) :-
	include(neighbour(XY), Board, Nghbs).

%tworzy planszę o zadanych wymiarach
createBoard(Width, Height, InitData, Board) :-
	findall(cell((X,Y),_,_,_),
		(between(1, Width, X),
		between(1, Height, Y)),
		Board),
	maplist(tie(Board), Board),
	maplist(init(InitData), Board).

%wypełnia komórkę danymi początkowymi
init(InitData, cell((X, Y), _, NR, W)) :-
	member((X, Y, Q), InitData),
	NR is Q - 1, %rozmiar stawu pomniejszamy o jeden, gdyż jedna komórka została już oznaczona jako jego część
	W is X * Y,  !.
init(_, _).


%SILNIK - TWORZENIE STAWÓW

%ok, czas na zabawę, czyli stworzenie stawu
%pierwszy argument: lista komórek, które są kandydatami na część tworzonego stawu
%drugi: ile komórek należy jeszcze dodać, aby ukończyć staw
%trzeci: numer budowanego stawu
iWannaLake(X, 0, LakeNr) :- !, %stworzyliśmy staw
	coverWithLeeve(X, LakeNr). %otoczmy go jeszcze groblą
iWannaLake([cell(_, Nghbs, _, X) | T], HowMany, LakeNr) :-
	var(X), 
	X = LakeNr, %aktualna komórka staje się częścią stawu
	checkThis(Nghbs), %jakaś taka heurystyka - sprawdzamy czy grobla się nie rozspójniła
	NHowMany is HowMany - 1,
	append(Nghbs, T, MoreNghbs), %a jej sąsiedzi potencjalnymi kandydatami
	iWannaLake(MoreNghbs, NHowMany, LakeNr).
iWannaLake([cell(_, Nghbs, _, leeve)|T], HowMany, LakeNr) :-
	!, 
	iWannaLake(T, HowMany, LakeNr).
iWannaLake([cell(_, _, _, LakeNr)|T], HowMany, LakeNr) :-
	iWannaLake(T, HowMany, LakeNr). %ta komórka już jest częścią naszego stawu. Pomiń

%wypełnianie sąsiadów stawu groblą. Proste
coverWithLeeve([], _) :- !.
coverWithLeeve([cell(_, Nghbs, _, leeve)|T], LakeNr) :-
	!, 
	coverWithLeeve(T, LakeNr).
coverWithLeeve([cell(_, _, _, LakeNr)|T], LakeNr) :-
	coverWithLeeve(T, LakeNr).

dfs([], FullBoard, LeeveSize) :- !,
	findLeeve(FullBoard, Leeve), %po stworzeniu stawów sprawdzamy spójność grobli 
	isLeeveOK([Leeve], LeeveSize).
dfs([cell(_, Nghbs, HowMany, Nr)|Tail], FullBoard, LeeveSize) :-
   iWannaLake(Nghbs, HowMany, Nr),
   dfs(Tail, FullBoard, LeeveSize).

%heurystyka sprawdzania spójności grobli działa mniej więcej tak:
%-rozważ sąsiadów komórki, która ma być stawem
%-jeśli dwa z nich są groblą to sprawdź czy istnieje między nimi ścieżka (idąc po grobli oczywiście)
checkThis(Ls) :-
	otherFindLeeve(Ls, Leeve, Tail), 
	iAmGood(Leeve, Tail).

iAmGood(_, []) :- !.
iAmGood(cell(_, Nghbs, _, leeve), Ls) :-
	otherFindLeeve(Ls, Leeve, Tail),
	Leeve \= pies,
	!,
	\+ (\+ (otherLeeveCheck(Nghbs, Leeve))),
	iAmGood(Leeve, Tail).
iAmGood(_, _).

%SPÓJNOŚĆ GROBLI

%znajdź komórkę, która NA PEWNO jest groblą (tj. jako grobli nie traktujemy nieukonkretnionej komórki)
%tak jak to ma miejsce w findLeeve
otherFindLeeve([], pies, []) :- !.
otherFindLeeve([Leeve|Tail], Leeve, Tail) :-
	Leeve = cell(_,_,_, X), nonvar(X), !.
otherFindLeeve([_|T], Leeve, Tail) :- otherFindLeeve(T, Leeve, Tail).

%znajduje dowolną komórkę będącą groblą
findLeeve([Leeve|_], Leeve) :-
	Leeve = cell(_,_,_, leeve), !.
findLeeve([_|T], Leeve) :- findLeeve(T, Leeve).

%sprawdzenie czy grobla jest spójna
%algorytm banalny - policz komórki do których możesz dojść idąć po grobli
%ich ilość powinna równać się żądanemu rozmiarowi grobli (czyli szerokość*wysokość - powierzchnia stawów)
isLeeveOK(_, 0) :- !. 
isLeeveOK([], _) :-  !, fail.
isLeeveOK([cell(_, Nghbs, X, leeve)|T], N) :-
	var(X),
	!,
	X = leeve,
	NN is N - 1,
	append(Nghbs, T, MoreNghbs),
	isLeeveOK(MoreNghbs, NN).
isLeeveOK([_|T], N) :- isLeeveOK(T, N).

%sprawdza czy z listy komórek (pierwszy argument) można dostać się do komórki będącej drugim argumentem
otherLeeveCheck([cell((X,Y),_,_,_)|_	], cell((X,Y),_,_,_)) :- !.
otherLeeveCheck([cell(_, Nghbs, X, leeve)|T], B) :-
	var(X),
	!,
	X = leeve,
	append(Nghbs, T, MoreNghbs),
	otherLeeveCheck(MoreNghbs, B).
otherLeeveCheck([_|T], B) :- otherLeeveCheck(T, B).

%WYJŚCIE

%robimy ASCII art
formatSolution([], []). 
formatSolution([cell(_,_,_, leeve)|T], ['*'|R]) :- !, formatSolution(T, R).
formatSolution([cell(_,_,Y,_)|T], ['_'|R]) :- var(Y), formatSolution(T, R).
formatSolution([cell(_,_,Y,_)|T], [Q|R]) :- nonvar(Y), Q is Y + 1, formatSolution(T, R).

%wypisz wiersz
rowToASCII(Tail, Tail, 0) :- nl.
rowToASCII([H|T], Tail, N) :-
	N > 0,
	write(H),
	write(' '),
	NN is N - 1,
	rowToASCII(T, Tail, NN).

%wypisz całą planszę
writeBoard(_, _, 0).
writeBoard(Board, Width, N) :-
	N > 0,
	rowToASCII(Board, Tail, Width),
	NN is N - 1,
	writeBoard(Tail, Width, NN).	
