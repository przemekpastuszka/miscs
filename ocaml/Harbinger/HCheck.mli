val checkElement:
	HTypes.langElement -> unit;;

val addOcamlFunction:
	(string*HTypes.varType list)->HTypes.varType->(HTypes.value list->HTypes.value)->unit;;
	
exception HNumberedErr of int*string;;
