(*****************************
Modu� odpowiadaj�cy za wykonanie kodu Harbinger
******************************)

open HTypes;;
open HVStack;;

exception HTemp of value;;
exception HNumberedErr of int*string;;
exception HBreak;;
exception HContinue;;

(*znajduje funkcj� w �rodowisku*)
let findTheFunction name types=
	(*try *) HData.get (name,types) HData.functions;;(* with
	Not_found-> raise (HInternalError ("HExecute","Unable to find desired function",name));;*)

(*zwraca typ wyra�enia*)
let rec getExpType expr=
	match expr with
	Number f->TNumber
	|Text t->TText
	| Bool b ->TBool
	| Variable v -> getVarType v
	| Fun (a,b) ->
		(
			if a="=="||a="!=" then
				TBool
			else
			
				match findTheFunction a (List.map (fun x->getExpType x) b) with
				OcamlF (_,_,t,_) -> t
				| HarF (Function (_,_,t,_,_)) -> t
		);;

(*pobiera warto�� i jednocze�nie sprawdza czy zosta�a ona zainicjalizowana*)
let getNCheckValue dsc=
	match getValue dsc with
	NotInitialized -> raise (HTemplateErr "Use of unitialized variable")
	|_ as q->q;;

(*proste wy�uskanie warto�ci bool*)
let getBoolV v=
	match v with
	VBool b -> b
	|_->raise (HInternalError ("HExecute","getBoolV",""));;

(*elementy tablicy mog� by� adresowane za pomoc� z�o�onego wyra�enia. Zadaniem tej funkcji
jest uproszczenie takiego wyra�enia do warto�ci typu number*)
let rec normalizeVar v=
	match v with
	VUsual _ as q -> q
	| VStructEl (a,s) ->VStructEl (normalizeVar a,s)
	| VArrEl (a,e) -> 
	(
		match evaluate e with
		VNumber f-> 
			if f<=0. then
				raise (HTemplateErr "Array elements can be accesed only using positive values");
			VArrEl (normalizeVar a,Number f)
		|_-> raise (HInternalError ("HExecute","normalizeVar",""))
	)		
and evaluate expr= (*oblicza warto�� wyra�enia*)
	match expr with
	Number f -> VNumber f
	| Text t -> VText t
	| Bool b -> VBool b
	| Variable v-> getNCheckValue (normalizeVar v)
	| Fun (a,b) -> 
		let valLs=List.map (fun x->evaluate x) b in		
		let types=List.map(fun x->getExpType x) b in
		executeFunc a valLs types b true
and execute (nr,stmt)= (*wykonaj instrukcj� j�zyka*)
	try
	(
		match stmt with
		Expression e -> ignore(evaluate e)
		| Creation (t,ls) -> List.iter (fun x-> create t x) ls			
		| Assignment (vr,exp) ->  update (normalizeVar vr) (evaluate exp)
				
		| While(exp,ls) ->
		begin
			openLvl(); (*tworzy nowy zasi�g dla zmiennych*)
			try while getBoolV (evaluate exp) do
				try List.iter execute ls with
					HContinue -> () (*napotkano instrukcj� continue*)
			done with
			HBreak -> (); (*napotkano instrukcj� break*)
			closeLvl();
		end
		
		| If (exp,ls) ->
		begin
			openLvl();
			if getBoolV (evaluate exp) then
				List.iter execute ls;
			closeLvl();
		end
		
		| IfElse (exp,ls1,ls2) ->  (*jak wy�ej*)
		begin
			openLvl();
			if getBoolV (evaluate exp) then
				List.iter execute ls1
			else
				List.iter execute ls2;
			closeLvl();
		end
				
		| For (st1,exp,st2,ls) ->
		begin
			execute (nr,st1);
			openLvl();
			try while getBoolV (evaluate exp) do
				(try List.iter execute ls with
					HContinue -> ());
				execute (nr,st2);
			done with
			HBreak -> ();
			closeLvl();
		end
	
		| Break -> raise HBreak
		| Continue -> raise HContinue
		| Return exp ->  raise (HTemp (evaluate exp))
	) 
	with HTemplateErr x -> raise (HNumberedErr (nr,x)) (*dodaj numer linii, w kt�rej wyst�pi� b��d*)
	
(*inicjuje i wykonuje funkcj� name z wyra�eniami b jako argumentami. Dodatkowo
otrzymuje parametr refAllowed, kt�ry pozwala na w��czenie i wy��czenie u�ycia referencji*)	
and executeFunc name valLs types b refAllowed=
	try begin
	match findTheFunction name types with
	OcamlF (_,_,_,f) -> f valLs
	| HarF (Function (_,_,_,args,stmts) ) ->
		(
			openFunc(); (*tworzy na stosie miejsce na now� funkcj�*)
			List.iter2 (fun x (y,z)->
				match x with
				Argument (s,t) -> (*przekazywanie przez warto��*)
					(
						create t s; 
						update (VUsual s) y;
					)
				| Reference (s,t) -> (*przez referencj�*)
					(
						if refAllowed then (*je�li referencje s� w��czone*)
							match z with
							Variable v -> HVStack.addRef v t s
							| _ -> raise(HInternalError ("HExecute","evaluate",""))
						else
							create t s; 
							update (VUsual s) y;
					)
				) args (List.map2 (fun x y-> (x,y)) valLs b);
			let result=ref (VBool true) in
			(try List.iter execute stmts with (*wykonanie funkcji*)
			HTemp v-> closeFunc();result:=v);
			!result
		)
	end with Not_found -> (*sprawdzanie r�wno�ci*)
		(
			if name="==" then
			(
				if (List.hd valLs)=(List.hd (List.tl valLs)) then
					VBool true
				else
					VBool false
			)		
			else if name="!=" then
			(
				if (List.hd valLs)=(List.hd (List.tl valLs)) then
					VBool false
				else
					VBool true
			)
					else raise (HInternalError ("HExecute","Cannot find desired function",name))
		);;
