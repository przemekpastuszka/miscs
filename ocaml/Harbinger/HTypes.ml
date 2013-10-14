(*Dostarcza informacje o typach i wyj�tkach u�ywanych przez interpreter*)

(*wyj�tki*)
exception HInternalError of string*string*string (*b��d wewn�trzny - nie powinien si� zdarzy�*)
exception HLanguageError of string*int*string
exception HSyntaxError of string*int*string
exception HTemplateErr of string;;
exception HNotFound

(*dost�pne typy zmiennych*)
type varType=
	| TBool
	| TText
	| TNumber
	| TArray of varType
	| TStruct of string

(*przechowuje warto�� zmiennej*)
type value=
	NotInitialized
	| VBool of bool
	| VText of string
	| VNumber of float	
	| VArray of value TreeArr.treeArray
	| VStruct of (string*value) list

type hVar=
	HVariable of varType*value

(*poniewa� odwo�anie do zmiennej mo�e by� z�o�one (tablica w strukturze itp)
wprowadzam odpowiedni typ do jego przechowywania*)
type var=
	VUsual of string
	|VStructEl of var*string
	|VArrEl of var*expr
and expr= (*dost�pne wyra�enia*)
	Number of float
	| Bool of bool
	| Text of string
	| Variable of var
	| Fun of string*expr list
	
type stmt= (*polecenia j�zyka Harbinger*)
	| Expression of expr (*pe�noprawne polecenie tylko w wypadku wywo�ania funkcji*)
	| Creation of varType*string list (*tworzy zmienne*)
	| Assignment of var*expr (*przypisuje warto�� zmiennej*)
	| If of expr*(int*stmt) list
	| IfElse of expr*(int*stmt) list*(int*stmt) list
	| While of expr*(int*stmt) list
	| For of stmt*expr*stmt*(int*stmt) list
	| Break
	| Continue
	| Return of expr

(*zar�wno struktury jak i funkcje mog� zosta� zadeklarowane i zdefiniowane, st�d pomocniczy typ*)
type hState=
	Defined
	| Declared

(*argument mo�na przekazywa� przez referencj� b�d� przez warto��*)
type argType=
	Reference of string*varType
	| Argument of string*varType

(*kolejno: stan funkcji (zadeklarowana,zdefiniowana),nazwa,typ zwracanego argumentu,lista argument�w,cia�o funkcji (sekwencja polece�)*)
type func=
	Function of hState*string*varType*argType list*(int*stmt) list
	
(*kolejno: stan struktury,nazwa oraz lista asocjacyjna zawieraj�ca nazwy p�l struktury i ich typy*)
type hStruct=
		HStruct of hState*string*(string*varType) list
	
(*definicje/deklaracje struktur i funkcji s� nadrz�dne wobec innych element�w j�zyka i jako jedyne s�u�� za entry points dla parsera*)
type langElement=
	InFunc of func*int
	| InStruct of hStruct*int
	
(*w �rodowisku wyst�puj� zadane funkcje Ocamla jak i funkcje zdefiniowane/zadeklarowane w plikach z kodem Harbinger*)
type funcType=
	OcamlF of string*varType list*varType*(value list->value)
	| HarF of func
