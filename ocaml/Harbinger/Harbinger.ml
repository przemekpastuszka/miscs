(*inicjalizacja �rodowiska*)
let _=HInit.init ();;

(*wyczy�� �rodowisko*)
let clearEnv ()=
	HData.clear HData.functions; (*usu� funkcje*)
	HData.clear HData.structs; (*usu� struktury*)
	HInit.init();; (*przeprowad� ponown� inicjalizacj�*)
	

(*otwarcie pliku z kodem Harbinger*)
let fopen fileName=
	let wej=open_in fileName in
	let lex=Lexing.from_channel wej in
	try
		begin
		while true do (*czytaj elementy dop�ki nie trafisz na eof*)
			try HCheck.checkElement (Parser.input Lexer.translate lex) with (*sprawd� poprawno�� elementu i dodaj go do �rodowiska*)
				HCheck.HNumberedErr (x,y)-> (*obs�uga b��d�w - czyszczenie �rodowiska i odpowiednie przygotowanie nowych b��d�w*)
						clearEnv();
						raise (HTypes.HLanguageError (fileName,x,y))
				| HTypes.HSyntaxError (_,x,y) ->
						clearEnv();
						raise (HTypes.HSyntaxError (fileName,x,y))
				| End_of_file -> raise End_of_file
				|_ as x->
						clearEnv();
						raise x
		done;
		close_in wej;
		end
	with
		End_of_file -> close_in wej;();;
		
let execute (name,types) valLs=
	HVStack.clear();
	HExecute.executeFunc name valLs types (List.map (fun x->HTypes.Bool true) valLs) false;;
	
let addFunction (name,types) ret f=
	HCheck.addOcamlFunction (name,types) ret f;;
