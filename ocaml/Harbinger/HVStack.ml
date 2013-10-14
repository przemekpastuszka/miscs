(*Obs³uga stosu u¿ywanego w trakcie wykonywania kodu Harbinger*)

open HTypes;;

type vType=
	Value of varType*value ref
	| Ref of varType*value ref*var list;;

let (stack:((string*vType) list list list ref))=ref [];;

(*dodaje v do aktualnego zasiêgu zmiennych w aktualnej funkcji*)
let addToCurrLvl v=
	match !stack with
	(a::b)::t-> stack:=((v::a)::b)::t
	|_-> raise (HInternalError ("HVStack","addToCurrentLvl","No local levels defined"));;

(*tworzy zmienn± name o typie v*)
let rec create v name=
	match v with
	TArray b->
		addToCurrLvl (name,Value (v,ref (VArray (TreeArr.empty()))))
	| TStruct t->
		addToCurrLvl (name,Value (v,ref (VStruct [])))
	| _-> addToCurrLvl (name,Value (v,ref NotInitialized));;

(*znajduje element o nazwie name*)
let rec getH name where=
	match where with
	h::t->
	(
		try List.assoc name h with
			Not_found -> getH name t
	)
	|_-> raise (HInternalError ("HVStack","get","Cannot find what are you looking for"));;	
	
(*znajduje el. o nazwie name w aktualnym zasiêgu*)
let getFromCurr name=
	match !stack with
	a::t-> getH name a	
	|_->  raise (HInternalError ("HVStack","getFromCurr","-"));;
	
(*zamienia zmienn± var na listê opisuj±c± t± zmienn±*)
let rec flatten v acc=
	match v with
	VUsual s as q -> q::acc
	| VStructEl (a,b) as q -> flatten a (q::acc)
	| VArrEl (a,b) as q -> flatten a (q::acc);;

(*zwraca typ zmiennej, opisanej przez dsc. Do wyszukania zmiennej stosowana jest funkcja f*)
let rec getVarTypeH f dsc=
	match dsc with
	VUsual s ->
	(
		match f s with
		Value (v,r) -> v
		| Ref (t,v,r) -> t
	)
	|VArrEl (v,_)-> 
	(
		match getVarTypeH f v with
		TArray t -> t
		|_-> raise (HInternalError ("HVStack","getVarType","1"))
	)
	|VStructEl (v,s)->
	(
		match getVarTypeH f v with
		TStruct a ->
			begin
						try
						(
							match HData.get a HData.structs with
									HStruct (_,_,ls) -> 
										try List.assoc s ls
											with Not_found ->  raise (HInternalError ("HVStack","getVarType","2"))
						)
						with Not_found ->  raise (HInternalError ("HVStack","getVarType","3"))
			end
		|_->   raise (HInternalError ("HVStack","getVarType","4"))
	);;

(*zwraca typ zmiennej znajduj±cej siê w aktualnym zasiêgu*)
let getVarType dsc=getVarTypeH getFromCurr dsc;;

(*zwraca nazwê funkcji*)
let rec getTheName v=
	match v with
	VUsual s -> s
	| VStructEl (vr,_) 
	| VArrEl (vr,_) -> getTheName vr  ;;

(*dodaje referencjê name do zmiennej typu t, opisanej przez v*)
let addRef v t name=
	match !stack with
	a::b::c->
		(
			let tmp=getH (getTheName v) b in
			(*let t=getVarTypeH (fun x->tmp) v in*)
			match tmp with
			Value (t2,vl) -> addToCurrLvl (name,Ref(t,vl,List.tl (flatten v [])))
			| Ref (t2,vl,ls) -> addToCurrLvl (name,Ref(t,vl,ls@(List.tl (flatten v []))))
			
		)
	|_-> raise (HInternalError ("HVStack","addRef","Cannot find what are you looking for"));;
		
	
(*
tzw. baardzo popl¹tany update
Przyjmuje: listê typu HTypes.var, która obrazuje schodzenie w g³¹b struktury
strukturê, na której wykonywany jest update
now¹ wartoœæ zmiennej
*)
let rec updateH ls upper vl=
	match ls with
	(VStructEl (a,b))::t as q-> 
	(
		match upper with (*update struktury: usuñ star¹ wartoœæ pod nazw¹ i dodaj now¹, obliczon¹ na podstawie starej wartoœci, nowej vl oraz pozosta³ej listy*) 
		VStruct str -> (try VStruct ((b,updateH t (List.assoc b str) vl)::(List.remove_assoc b str)) with
		Not_found -> (*co jeœli próbujemy przypisaæ coœ niezainicjalizowanej strukturze?*)
		(
			match t with (*inicjujemy j¹*)
			[] -> VStruct ((b,vl)::str)
			| (VArrEl _)::a -> VStruct ((b,updateH t (VArray(TreeArr.empty())) vl)::str)
			| (VStructEl _)::a ->  VStruct ((b,updateH t (VStruct []) vl)::str)
			|_ -> raise (HInternalError ("HVStack","updateH","01"))
		))
		| NotInitialized -> updateH q (VStruct []) vl
		|_ -> raise (HInternalError ("HVStack","updateH","1"))
	)
	| (VArrEl (a,(Number f)))::t as q-> 
	(
		match upper with
		VArray arr-> (try VArray (TreeArr.update arr (int_of_float(f),updateH t (TreeArr.get arr (int_of_float f)) vl) NotInitialized) with
			TreeArr.IndexOutOfBounds -> 
			(
				match t with
				[] -> VArray (TreeArr.update arr  (int_of_float(f),vl) NotInitialized)
				| (VArrEl _)::a -> VArray (TreeArr.update arr (int_of_float(f),updateH t (VArray(TreeArr.empty())) vl) NotInitialized)
				| (VStructEl _)::a -> VArray (TreeArr.update arr (int_of_float(f),updateH t (VStruct []) vl) NotInitialized)
				|_ -> raise (HInternalError ("HVStack","updateH","02"))
			))
		| NotInitialized -> updateH q (VArray (TreeArr.empty())) vl
		|_ -> raise (HInternalError ("HVStack","updateH","2"))
	)
	| [] -> vl
	| _ ->  raise (HInternalError ("HVStack","updateH","3"));;
	
(*update zmiennej opisanej przez dsc warto¶ci± vl*)
let update dsc vl=
	let ls=flatten dsc [] in
	match ls with
	(VUsual s)::t ->
	(
		match getFromCurr s with
		Value (v,r) ->	
			r:=updateH t (!r) vl
		| Ref (tp,v,ls) ->
			v:=updateH (ls@t) (!v) vl
	)
	|_-> raise (HInternalError ("HVStack","update","-"));;

(*zwróc warto¶æ zmiennej opisanej list± ls. Upper to nadrzêdna zmienna*)
let rec getValueLs ls upper=
	match ls with
	(VStructEl (a,b))::t ->
	(
		match upper with
		VStruct str ->
		begin
			try getValueLs t (List.assoc b str) with
			Not_found-> raise (HTemplateErr "Use of unitialized variable")
		end
		| NotInitialized -> raise (HTemplateErr "Use of unitialized variable")
		| _-> raise (HInternalError ("HVStack","getValueLs","1"))
	)
	|(VArrEl (a,Number f))::t ->
	(
		match upper with
		VArray arr ->
		begin
			try getValueLs t (TreeArr.get arr (int_of_float f)) with
			TreeArr.IndexOutOfBounds -> raise (HTemplateErr "Use of unitialized variable")
		end
		| NotInitialized -> raise (HTemplateErr "Use of unitialized variable")
		| _-> raise (HInternalError ("HVStack","getValueLs","2"))
	)
	| [] -> upper
	| _ ->  raise (HInternalError ("HVStack","getValueLs","3"));;

(*zwraca warto¶æ zmiennej opisanej przez desc*)
let getValue dsc=
	let ls=flatten dsc [] in
	match ls with
	(VUsual s)::t ->
	(
		match getFromCurr s with
		Value (v,r) ->	
			getValueLs t (!r)
		| Ref (tp,v,ls) ->
			getValueLs (ls@t) (!v)
	)
	|_-> raise (HInternalError ("HVStack","getValue","-"));;
	
(*operacje na strukturze stosu*)
let clear ()=
	stack:=[];;

let openFunc ()=
	stack:=[[]]::(!stack);;

let closeFunc ()=
	match !stack with
	h::t->stack:=t
	|[]->stack:=[];;
	
let openLvl ()=
	match !stack with
	h::t->stack:=([]::h)::t
	|[]-> raise (HInternalError ("HVStack","openLvl","-"));;
	
let closeLvl ()=
	match !stack with
	(a::b)::t->stack:=b::t
	|_-> raise (HInternalError ("HVStack","closeLvl","-"));;
