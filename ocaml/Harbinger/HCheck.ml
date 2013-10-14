(*****************************
Tutaj wykonywane jest sprawdzenie poprawno�ci zadanej funkcji
*****************************)

open HTypes;;

(*wyj�tki tymczasowe*)
exception HTemplateErr of string
exception HNumberedErr of int*string



(***************************************************************
sprawd� typ wyra�enia - konkretnie: czy nie u�yto niezadeklarowanej struktury
***************************************************************)
let rec checkType t=
	match t with
	TArray q -> checkType q
	| TStruct s -> (try ignore(HData.get s HData.structs) with Not_found -> raise (HTemplateErr ("Undeclared structure '"^s^"'")))
	| _ -> ();;
(*********************************************************************************************************)



(****************************************************************************
funckcja pomocnicza
*****************************************************************************)	
let rec searchForVar ls v=
	match ls with
	h::t ->
		(
			try List.assoc v h with
			Not_found -> searchForVar t v
		)
	| [] -> raise Not_found;;
(**************************************************************************************)
	
let checkRef v param=
	match param with
	Argument(_,_) -> ()
	| Reference (_,_) ->
		(
			match v with
			Variable _ -> ()
			|_-> raise (HTemplateErr ("Only variable can be used as a reference argument"))
		);;
	
	
(*************************************************************************************
sprawdza poprawno�� i zwraca typ wyra�enia
argumentami s�: wyra�enie, lista aktualnych zmiennych
*************************************************************************************)
let rec getExpType exp vs=
	match exp with
	Number a -> TNumber
	| Bool a -> TBool
	| Text a -> TText
	| Fun (a,b) -> 
		(
			try
				begin	(*znajd� zadan� funkcj� w �rodowisku i zwr�c jej typ (je�li istnieje)*)
					match HData.get (a,List.map (fun x -> getExpType x vs) b) HData.functions with
					OcamlF (_,_,t,_) -> t
					| HarF (Function (_,_,t,ls,_)) -> 
						List.iter2 (fun x y->checkRef x y) b ls; (*sprawd� poprawno�� argument�w dla funkcji*)
						t
				end
			with Not_found -> 
				begin
					match a with
						"==" | "!=" -> (*operatory 'r�wne' i 'r�ne' s� wyr�nione ze wzgl�du na to, i� mog� zosta� u�yte w kontek�cie dowolnych dw�ch wyra�e� tego samego typu*)
								(	
									if List.length b<>2 then (*operatory == i != s� binarne*)
										raise (HInternalError ("HCheck","getExpType","== or != used as non-binary operator"));
										
									(*sprawd� czy wyra�enia stoj�ce po prawej i lewej stronie operatora s� tego samego typu*)
									if getExpType (List.hd b) vs<>getExpType (List.hd (List.tl b)) vs then
										raise (HTemplateErr  ("Undeclared function '"^a^"'"));
									TBool
								)
						|_-> raise (HTemplateErr ("Undeclared function '"^a^"'"))
				end
		)
	| Variable (VUsual b) -> 
			 (try searchForVar vs b with Not_found -> raise (HTemplateErr  ("Undefined variable '"^b^"'"))) (*je�li zmienna to po prostu identyfikator*)
			 
	| Variable (VArrEl (c,b)) -> (*je�li to element tablicy*)
			begin
					if getExpType b vs<>TNumber then (*m�wi samo za siebie*)
						raise (HTemplateErr  "Array elements can be accesed only using numbers");
								
					match getExpType (Variable c) vs with (*to chyba te�*)
							TArray t -> t
							| _ ->raise (HTemplateErr  "Array operators '[]' applied to non-array variable")
			end
			
	| Variable (VStructEl (c,b)) -> (*je�li struktura*)
			begin
				match getExpType (Variable c) vs with
					TStruct s -> 
							begin
								try
								(
									match HData.get s HData.structs with
											HStruct (_,_,ls) -> 
												try List.assoc b ls
													with Not_found -> raise (HTemplateErr  ("'"^b^"' is not an element of '"^s^"'"))
								)
								with Not_found -> raise (HTemplateErr  ("Undeclared structure '"^s^"'"))
							end
					| _ -> raise (HTemplateErr  "Structure operator '.' applied to non-structure variable")
			end
		
(*******************************************************************************************************)		


(******************************************************************************
Sprawdza poprawno�� wyra�enia j�zyka
****************************************************************************)
let rec checkStmt (nr,st) ret v=
	try
	(
		match st with
		Expression e -> (*zwyk�e wyra�enie, wystarczy sprawdzi� wewn�trzn� poprawno��*)
				ignore(getExpType e v);
				[]
				
		| Creation (t,ls) -> (*tworzenie zmiennej*)
				checkType t; (*sprawd� typ*)
				(*sprawd� czy nie ma pr�by utworzenia ju� istniej�cej zmiennej*)
				List.iter (fun x -> if List.mem_assoc x (List.hd v) then raise (HTemplateErr  ("Redeclaration of variable '"^x^"'"))) ls;
				List.map (fun x-> (x,t)) ls (*zwr�c list� asocjacyjn� z nowymi zmiennymi*)
				
		| Assignment (vr,exp) ->  (*sprawd� czy typ zmiennej zgadza si� z typem przypisywanego wyra�enia*)
				if getExpType (Variable vr) v<>getExpType exp v then raise (HTemplateErr  "Types mismatch"); 
				[]
				
		(*sytuacja while jest identyczna z if*)
		| While(exp,ls)
		| If (exp,ls) -> 
				if getExpType exp v<>TBool then raise (HTemplateErr  "Type bool expected"); (*sprawd� czy warunek jest typu bool*)
				ignore(List.fold_left (fun x y->(checkStmt y ret (x::v))@x) [] ls); (*sprawd� wszystkie wyra�enia pomi�dzy klamrami*)
				[]
				
		| IfElse (exp,ls1,ls2) ->  (*jak wy�ej*)
				if getExpType exp v<>TBool then raise (HTemplateErr "Type bool expected"); 
				ignore(List.fold_left (fun x y->(checkStmt y ret (x::v))@x) [] ls1);
				ignore(List.fold_left (fun x y->(checkStmt y ret (x::v))@x) [] ls2);
				[]
				
		| For (st1,exp,st2,ls) -> (*podobnie jak wy�ej - jedynym dodatkiem jest sprawdzenie poprawno�ci wyra�enia inicjalizuj�cego i wykonywanego przy iteracji*)
				ignore(checkStmt (nr,st1) ret v);
				ignore(checkStmt (nr,st2) ret v); 
				if getExpType exp v<>TBool then raise (HTemplateErr  "Type bool expected");
				ignore(List.fold_left (fun x y->(checkStmt y ret (x::v))@x) [] ls);
				[]
				
		| Break
		| Continue -> 
				[]
				
		| Return exp ->  (*sprawdzenie czy zwracane jest wyra�enie odpowiedniego typu*)
				if getExpType exp v<>ret then raise (HTemplateErr  "Function returns value of different type");
				[]
	) 
	with HTemplateErr x -> raise (HNumberedErr (nr,x));; (*dodaj numer linii, w kt�rej wyst�pi� b��d*)
(************************************************************************************)



(*************************************************************************
Sprawdza poprawno�� u�ycia s��w kluczowych break i continue
*****************************************************************************)
let rec checkFlow (nr,st) =
		match st with
		If (_,ls)-> List.iter (fun x->checkFlow x) ls
		| IfElse (_,ls1,ls2) -> List.iter (fun x->checkFlow x) ls1; List.iter (fun x->checkFlow x) ls2
		| Break | Continue -> raise (HNumberedErr (nr,"This keyword can be used only inside loop"))
		| _ -> ();;
(**********************************************************************************)		
		

	
(************************************************************************************
Sprawdza czy wyra�enie zwraca warto��
************************************************************************************)
let rec checkRet (nr,st) =
		match st with 
		IfElse (_,ls1,ls2) -> (List.fold_left (fun x y->(checkRet y)||x) false ls1)&&(List.fold_left (fun x y->(checkRet y)||x) false ls2)
		| Return _ -> true
		| _ -> false;;
(**********************************************************************************)			


let rec checkDuplicates ls nr=
	match ls with
	(a,b)::t-> 
		if List.mem_assoc a t then
			raise (HNumberedErr (nr,"Variable '"^a^"' is declared more than once"));
		checkDuplicates t nr
	| [] -> ();;
	
let flatArg arg=
	match arg with
	Reference (a,b)
	| Argument (a,b)-> (a,b);;
	
let checkElement x=
match x with
	InFunc (Function (state,name,ret,args,stmts),nr) ->
		(
			let v=ref [] in
			let argTypes=List.map 
				(fun x-> match x with
					Reference (x,y)
					| Argument (x,y) -> v:=(x,y)::(!v);y) args in
				(try (*sprawd� czy w �rodowisku istnieje ju� taka funkcja*)
				(
					match HData.get (name,argTypes) HData.functions with
					OcamlF (nState,_,_,_) -> raise (HNumberedErr (nr,"Redefinition of a function '"^name^"'"))
					| HarF(Function(nState,_,nRet,_,_)) ->
						(
							if state=Defined then
								match nState with
								Defined -> raise (HNumberedErr (nr,"Redefinition of a function '"^name^"'"))
								| Declared -> 
									(
										if ret<>nRet then
											raise (HNumberedErr (nr,"Definition and declaration of function '"^name^"' differs about returned value"));
										checkDuplicates (List.map flatArg args) nr;
										HData.update ((name,argTypes),HarF (Function (state,name,ret,args,stmts))) HData.functions
									)
						)
				) with
				Not_found -> checkDuplicates (List.map flatArg args) nr; HData.add ((name,argTypes),HarF (Function (state,name,ret,args,stmts))) HData.functions);
				if state=Defined then
					begin
						ignore(List.fold_left (fun x y->(checkStmt y ret [x])@x) (!v) stmts);
						List.iter (fun x -> checkFlow x) stmts;
						if (List.fold_left (fun x y->(checkRet y)||x) false stmts)=false then
							raise (HNumberedErr (nr,"In function '"^name^"' - not all code paths return a value"))
					end
		)
	| InStruct (HStruct (state,name,el),nr) -> 
		(
			(try 
				(
					match HData.get name HData.structs with
					HStruct (nState,_,_) ->
						(
							if state=Defined then
								match nState with
								Defined -> raise (HNumberedErr (nr,"Redefinition of a structure '"^name^"'"))
								| Declared -> checkDuplicates el nr;HData.update (name,HStruct (state,name,el)) HData.structs
						)
				) with
				Not_found -> checkDuplicates el nr; HData.add (name,HStruct (state,name,el)) HData.structs);
				if state=Defined then
					List.iter (fun (s,t) -> checkType t) el;
		);;
		
let addOcamlFunction (name,types) ret f=
	try
	match HData.get (name,types) HData.functions with
		OcamlF (nState,_,_,_) -> raise (HNumberedErr (0,"Redefinition of a function '"^name^"'"))
		| HarF(Function(Defined,_,nRet,_,_)) -> raise (HNumberedErr (0,"Redefinition of a function '"^name^"'"))
		| HarF(Function(Declared,_,nRet,_,_)) ->
			(
				if ret<>nRet then
					raise (HNumberedErr (0,"Definition and declaration of function '"^name^"' differs about returned value"));
				HData.update ((name,types),OcamlF (name,types,ret,f)) HData.functions
			)
	with
	Not_found->HData.add ((name,types),OcamlF (name,types,ret,f)) HData.functions;;
	
