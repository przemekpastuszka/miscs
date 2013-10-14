{
	open Parser
	
	(*aktualizuje strukturê przechowuj¹c¹ informacje o pozycji w pliku*)
	let incr_linenum lexbuf =
		let pos = lexbuf.Lexing.lex_curr_p in
		lexbuf.Lexing.lex_curr_p <- { pos with
		  Lexing.pos_lnum = pos.Lexing.pos_lnum + 1;
		  Lexing.pos_bol = pos.Lexing.pos_cnum};;
	  
	 (*odwraca string*) 
	let rec rev s =
		let n = String.length s in
		for i=0 to (n - 1) / 2 do
			let c = s.[i] in
			s.[i] <- s.[n-i-1];
			s.[n-i-1] <- c;
		done;
	s;;
	
	(*tablica hashuj¹ca dla s³ów kluczowych*)
	let create_hashtable size init =
		let tbl = Hashtbl.create size in
		List.iter (fun (key, data) -> Hashtbl.add tbl key data) init;
		tbl;;

	 let keyword_table = 
		create_hashtable 14 
		[
		  ("if", If);
		  ("else", Else);
		  ("function", Function);
		  ("array", Array);
		  ("number", Number);
		  ("bool", Bool);
		  ("text", Text);
		  ("struct", Struct);
		  ("ref", Reference);
		  ("for", For);
		  ("while", While);
		  ("continue", Continue);
		  ("break", Break);
		  ("return", Return);
		]

 }

 (*g³ówny entrypoint*)
rule translate = parse
  | ['0'-'9']+
  | ['0'-'9']+ '.' ['0'-'9']+ as x {CNumber (float_of_string(x))}
  |  '"' {CText (text "" lexbuf)}
  | "true"
  | "false" as x {CBool (bool_of_string(x))}
  | '{' { LBracket }
  | '}' {RBracket}
  | '(' {LParent}
  | ')' {RParent}
  | '[' {LArr}
  | ']' {RArr}
  | "/*" {comment 0 lexbuf}
  | "=" { Assignment}
  | "*"
  | "/"
  | "%" as op {MulDivMod (Char.escaped op)}
  | "+" {Plus "+"}
  | "-" {Minus "-"}
  | "=="
  | "!="
  | "<"
  | ">"
  |"<="
  |">=" as op {EqGr op}
  | "||"
  | "&&" as op {Logic op}
  | ';' { Semicolon}
  | ',' {Coma}
  | '.' {Dot}
  | ['a'-'z' 'A'-'Z'] ['a'-'z' '_' 'A'-'Z' '0'-'9']* as wrd
				{
						try Hashtbl.find keyword_table wrd with
							Not_found -> Identifier wrd
				}
  | "//" [^ '\n']* {translate lexbuf} (*komentarz - usuñ wszystkie znaki do znaku nowej linii*)
  | [' ' '\t'] {translate lexbuf } (*pomiñ bia³e znaki*)
  | '\n' {incr_linenum lexbuf;translate lexbuf}
  | _ as c {print_char c; translate lexbuf}
  | eof { raise End_of_file }
  
 (*pomija zagnie¿d¿one komentarze*) 
and comment lvl=parse
  | '\n' {incr_linenum lexbuf;comment lvl lexbuf}
  | "*/" { if lvl=0 then translate lexbuf
				else
					comment (lvl-1) lexbuf }
  | "/*" { comment (lvl+1) lexbuf }
  | _ { comment lvl lexbuf}
  | eof {raise End_of_file }

 (*parsuje tekst*)
and text res=parse
	| '\n' {incr_linenum lexbuf;text res lexbuf}
	| '"' {rev res}
	| '\\' '\\' {text ("\\"^res) lexbuf} (*dwa backslashe pod rz¹d - dodajemy do wyniku jeden*)
	| '\\' '"' {text ("\""^res) lexbuf} (*backslash przed znakiem " - dodajemy znak "*)
	| _ as c {text ((Char.escaped c)^res) lexbuf}
	| eof {raise End_of_file}
	
 
