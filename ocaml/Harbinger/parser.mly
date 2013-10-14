%{
	open Lexing
	
	let getLine ()=(Parsing.symbol_start_pos ()).pos_lnum;;
	
	let showError str=
		raise (HTypes.HSyntaxError ("",getLine(),str));;
		
	
%}

%token <float> CNumber
%token <string> CText
%token <bool> CBool
%token Number Text Bool Array Struct
%token <string> Identifier
%token <string> Logic EqGr Plus Minus MulDivMod Pow
%token Function Return
%token While For Continue Break 
%token If Else 
%token Semicolon Coma Dot
%token Assignment
%token Reference
%token LParent RParent
%token LArr RArr
%token LBracket RBracket

%left Logic
%left EqGr
%left Plus Minus
%left MulDivMod
%left Neg
%right Pow

%start input
%type <HTypes.langElement> input

%%

input: func {HTypes.InFunc ($1,getLine())}
	| strct {HTypes.InStruct ($1,getLine())}
	| error {showError "Function or structure definition/declaration expected"}
;

vartype: 
	Bool {HTypes.TBool}
	| Text {HTypes.TText}
	| Number {HTypes.TNumber}
	| Array LArr vartype RArr {HTypes.TArray $3}
	| Struct Identifier {HTypes.TStruct $2}
;

idents:
	Identifier {[$1]}
	| idents Coma Identifier {$3::$1}
;

varCreation:
	vartype idents Semicolon {List.map (fun x->(x,$1)) $2}
;

field:
	varCreation {$1}
	| field varCreation {$2@$1}
	| error  {showError "Invalid structure field"}
;

strct:
	|Struct Identifier Semicolon {HTypes.HStruct (HTypes.Declared,$2,[])}
	|Struct Identifier LBracket field RBracket {HTypes.HStruct (HTypes.Defined,$2,$4)}
	|Struct {showError "Invalid structure definition/declaration"}
;

paramlist: 
	 Reference vartype Identifier {[HTypes.Reference ($3,$2)]}
	| vartype Identifier {[HTypes.Argument ($2,$1)]}
	| paramlist Coma Reference vartype Identifier {HTypes.Reference ($5,$4)::$1}
	| paramlist Coma vartype Identifier {HTypes.Argument ($4,$3)::$1}
	| error  {showError "Invalid list of parameters"}
;	

chosenOperators:
	| Plus {$1}
	| Minus {$1}
	| MulDivMod {$1}
	| Pow {$1}
;

operator:
	Logic {$1}
	| EqGr {$1}
	| chosenOperators {$1}
;

func: 
	Function vartype Identifier LParent paramlist RParent Semicolon {HTypes.Function (HTypes.Declared,$3,$2,List.rev $5,[])}
	| Function vartype Identifier LParent RParent Semicolon {HTypes.Function (HTypes.Declared,$3,$2,[],[])}
	| Function vartype Identifier LParent paramlist RParent LBracket seq RBracket {HTypes.Function (HTypes.Defined,$3,$2,List.rev $5,List.rev $8)}
	| Function vartype Identifier LParent RParent LBracket seq RBracket  {HTypes.Function (HTypes.Defined,$3,$2,[],List.rev $7)}
	| Function vartype operator LParent paramlist RParent Semicolon {HTypes.Function (HTypes.Declared,$3,$2,List.rev $5,[])}
	| Function vartype operator LParent paramlist RParent LBracket seq RBracket {HTypes.Function (HTypes.Defined,$3,$2,List.rev $5,List.rev $8)}
	| Function error {showError "Invalid function definition/declaration"}
;

arglist:
	expr {[$1]}
	| arglist Coma expr {$3::$1}
;

var:
	Identifier {HTypes.VUsual $1}
	| var Dot Identifier {HTypes.VStructEl ($1,$3)}
	| var LArr expr RArr {HTypes.VArrEl ($1,$3)}
;

expr:
	CNumber {HTypes.Number $1}
	|CText {HTypes.Text $1}
	|CBool {HTypes.Bool $1}
	|var {HTypes.Variable $1}
	|Identifier LParent RParent {HTypes.Fun ($1,[])}
	|Identifier LParent arglist RParent {HTypes.Fun ($1,List.rev $3)}
	|expr Logic expr {HTypes.Fun ($2,[$1;$3])}
	|expr EqGr expr {HTypes.Fun ($2,[$1;$3])}
	|expr Plus expr {HTypes.Fun ($2,[$1;$3])}
	|expr Minus expr {HTypes.Fun ($2,[$1;$3])}
	|expr MulDivMod expr {HTypes.Fun ($2,[$1;$3])}
	|expr Pow expr {HTypes.Fun ($2,[$1;$3])}
	|Minus expr %prec Neg {HTypes.Fun ($1,[$2])}
	|LParent expr RParent {$2}
	| error {showError "Invalid expression"}
;



stmt:
	|Identifier LParent RParent Semicolon {(getLine(),HTypes.Expression (HTypes.Fun ($1,[])))}
	|Identifier LParent arglist RParent Semicolon {(getLine(),HTypes.Expression (HTypes.Fun ($1,List.rev $3)))}
	| vartype idents Semicolon {(getLine(),HTypes.Creation ($1,$2))}
	| var Assignment expr Semicolon {(getLine(),HTypes.Assignment ($1,$3))}
	| var chosenOperators Assignment expr Semicolon {(getLine(),HTypes.Assignment ($1,HTypes.Fun ($2,[HTypes.Variable $1;$4])))}
	| If LParent expr RParent LBracket seq RBracket {(getLine(),HTypes.If ($3,List.rev $6))}
	| If LParent expr RParent LBracket seq RBracket Else LBracket seq RBracket {(getLine(),HTypes.IfElse ($3,List.rev $6,List.rev $10))}
	| While LParent expr RParent LBracket seq RBracket {(getLine(),HTypes.While ($3,List.rev $6))}
	| For LParent var Assignment expr Semicolon expr Semicolon var Assignment expr RParent LBracket seq RBracket 
		{(getLine(),HTypes.For (HTypes.Assignment ($3,$5),$7,HTypes.Assignment ($9,$11),List.rev $14))}
	| Break Semicolon {(getLine(),HTypes.Break )}
	| Continue Semicolon {(getLine(),HTypes.Continue )}
	| Return expr Semicolon {(getLine(),HTypes.Return $2)}
	| error {showError "Invalid language statement"}
;
seq:
	stmt {[$1]}
	| seq stmt {$2::$1}
;

%%

