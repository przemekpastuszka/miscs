(*Dostarcza informacje o typach i wyj±tkach u¿ywanych przez interpreter*)

(*wyj¹tki*)
exception HInternalError of string*string*string (*b³¹d wewnêtrzny - nie powinien siê zdarzyæ*)
exception HLanguageError of string*int*string
exception HSyntaxError of string*int*string
exception HTemplateErr of string;;
exception HNotFound

(*dostêpne typy zmiennych*)
type varType=
	| TBool
	| TText
	| TNumber
	| TArray of varType
	| TStruct of string

(*przechowuje wartoœæ zmiennej*)
type value=
	NotInitialized
	| VBool of bool
	| VText of string
	| VNumber of float	
	| VArray of value TreeArr.treeArray
	| VStruct of (string*value) list

type hVar=
	HVariable of varType*value

(*poniewa¿ odwo³anie do zmiennej mo¿e byæ z³o¿one (tablica w strukturze itp)
wprowadzam odpowiedni typ do jego przechowywania*)
type var=
	VUsual of string
	|VStructEl of var*string
	|VArrEl of var*expr
and expr= (*dostêpne wyra¿enia*)
	Number of float
	| Bool of bool
	| Text of string
	| Variable of var
	| Fun of string*expr list
	
type stmt= (*polecenia jêzyka Harbinger*)
	| Expression of expr (*pe³noprawne polecenie tylko w wypadku wywo³ania funkcji*)
	| Creation of varType*string list (*tworzy zmienne*)
	| Assignment of var*expr (*przypisuje wartoœæ zmiennej*)
	| If of expr*(int*stmt) list
	| IfElse of expr*(int*stmt) list*(int*stmt) list
	| While of expr*(int*stmt) list
	| For of stmt*expr*stmt*(int*stmt) list
	| Break
	| Continue
	| Return of expr

(*zarówno struktury jak i funkcje mog¹ zostaæ zadeklarowane i zdefiniowane, st¹d pomocniczy typ*)
type hState=
	Defined
	| Declared

(*argument mo¿na przekazywaæ przez referencjê b¹dŸ przez wartoœæ*)
type argType=
	Reference of string*varType
	| Argument of string*varType

(*kolejno: stan funkcji (zadeklarowana,zdefiniowana),nazwa,typ zwracanego argumentu,lista argumentów,cia³o funkcji (sekwencja poleceñ)*)
type func=
	Function of hState*string*varType*argType list*(int*stmt) list
	
(*kolejno: stan struktury,nazwa oraz lista asocjacyjna zawieraj¹ca nazwy pól struktury i ich typy*)
type hStruct=
		HStruct of hState*string*(string*varType) list
	
(*definicje/deklaracje struktur i funkcji s¹ nadrzêdne wobec innych elementów jêzyka i jako jedyne s³u¿¹ za entry points dla parsera*)
type langElement=
	InFunc of func*int
	| InStruct of hStruct*int
	
(*w œrodowisku wystêpuj¹ zadane funkcje Ocamla jak i funkcje zdefiniowane/zadeklarowane w plikach z kodem Harbinger*)
type funcType=
	OcamlF of string*varType list*varType*(value list->value)
	| HarF of func
