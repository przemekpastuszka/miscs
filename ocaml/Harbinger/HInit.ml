open HTypes;;

(*dodaj do œrodowiska najbardziej podstawowe funkcje (operatory arytmetyczne i logiczne)*)

let init()=
	HData.add (("+",[TNumber;TNumber]),OcamlF ("+",[TNumber;TNumber],TNumber,
		fun x->  match x with
			(VNumber a)::(VNumber b)::[] ->VNumber (a+.b)
			| _ -> raise (HInternalError ("HInit","init","+"))))  HData.functions;
	
	HData.add (("-",[TNumber;TNumber]),OcamlF ("-",[TNumber;TNumber],TNumber,
		fun x->  match x with
			(VNumber a)::(VNumber b)::[] ->VNumber (a-.b)
			| _ -> raise (HInternalError ("HInit","init","-"))))  HData.functions;
			
	HData.add (("-",[TNumber]),OcamlF ("-",[TNumber],TNumber,
		fun x->  match x with
			(VNumber a)::[] ->VNumber (-.a)
			| _ -> raise (HInternalError ("HInit","init","unary -"))))  HData.functions;
			
HData.add (("*",[TNumber;TNumber]),OcamlF ("*",[TNumber;TNumber],TNumber,
		fun x->  match x with
			(VNumber a)::(VNumber b)::[] ->VNumber (a*.b)
			| _ -> raise (HInternalError ("HInit","init","*"))))  HData.functions;
			
	HData.add (("/",[TNumber;TNumber]),OcamlF ("/",[TNumber;TNumber],TNumber,
		fun x->  match x with
			(VNumber a)::(VNumber b)::[] ->VNumber (a/.b)
			| _ -> raise (HInternalError ("HInit","init","/"))))  HData.functions;
			
	HData.add (("^",[TNumber;TNumber]),OcamlF ("^",[TNumber;TNumber],TNumber,
		fun x->  match x with
			(VNumber a)::(VNumber b)::[] ->VNumber (a**b)
			| _ -> raise (HInternalError ("HInit","init","^"))))  HData.functions;
			
	HData.add (("%",[TNumber;TNumber]),OcamlF ("%",[TNumber;TNumber],TNumber,
		fun x->  match x with
			(VNumber a)::(VNumber b)::[] ->VNumber (float_of_int((int_of_float(a)) mod (int_of_float(b))))
			| _ -> raise (HInternalError ("HInit","init","%"))))  HData.functions;
			
	HData.add (("<",[TNumber;TNumber]),OcamlF ("<",[TNumber;TNumber],TBool,
		fun x->  match x with
			(VNumber a)::(VNumber b)::[] ->VBool (a<b)
			| _ -> raise (HInternalError ("HInit","init","<"))))  HData.functions;
	
	HData.add ((">",[TNumber;TNumber]),OcamlF (">",[TNumber;TNumber],TBool,
		fun x->  match x with
			(VNumber a)::(VNumber b)::[] ->VBool (a>b)
			| _ -> raise (HInternalError ("HInit","init",">"))))  HData.functions;
	
	HData.add (("<=",[TNumber;TNumber]),OcamlF ("<=",[TNumber;TNumber],TBool,
		fun x->  match x with
			(VNumber a)::(VNumber b)::[] ->VBool (a<=b)
			| _ -> raise (HInternalError ("HInit","init","<="))))  HData.functions;
	
	HData.add ((">=",[TNumber;TNumber]),OcamlF (">=",[TNumber;TNumber],TBool,
		fun x->  match x with
			(VNumber a)::(VNumber b)::[] ->VBool (a>=b)
			| _ -> raise (HInternalError ("HInit","init",">="))))  HData.functions;
			
			
	HData.add (("<",[TText;TText]),OcamlF ("<",[TText;TText],TBool,
		fun x->  match x with
			(VText a)::(VText b)::[] ->VBool (a<b)
			| _ -> raise (HInternalError ("HInit","init","< TText"))))  HData.functions;
	
	HData.add ((">",[TText;TText]),OcamlF (">",[TText;TText],TBool,
		fun x->  match x with
			(VText a)::(VText b)::[] ->VBool (a>b)
			| _ -> raise (HInternalError ("HInit","init","> TText"))))  HData.functions;
	
	HData.add (("<=",[TText;TText]),OcamlF ("<=",[TText;TText],TBool,
		fun x->  match x with
			(VText a)::(VText b)::[] ->VBool (a<=b)
			| _ -> raise (HInternalError ("HInit","init","<= TText"))))  HData.functions;
	
	HData.add ((">=",[TText;TText]),OcamlF (">=",[TText;TText],TBool,
		fun x->  match x with
			(VText a)::(VText b)::[] ->VBool (a>=b)
			| _ -> raise (HInternalError ("HInit","init",">= TText"))))  HData.functions;
		
	HData.add (("+",[TText;TText]),OcamlF ("+",[TText;TText],TText,
		fun x->  match x with
			(VText a)::(VText b)::[] ->VText (a^b)
			| _ -> raise (HInternalError ("HInit","init","+ TText"))))  HData.functions;
	
	HData.add (("&&",[TBool;TBool]),OcamlF ("&&",[TBool;TBool],TBool,
		fun x->  match x with
			(VBool a)::(VBool b)::[] ->VBool (a&&b)
			| _ -> raise (HInternalError ("HInit","init","&&"))))  HData.functions;
			
	HData.add (("||",[TBool;TBool]),OcamlF ("||",[TBool;TBool],TBool,
		fun x->  match x with
			(VBool a)::(VBool b)::[] ->VBool (a||b)
			| _ -> raise (HInternalError ("HInit","init","||"))))  HData.functions;;