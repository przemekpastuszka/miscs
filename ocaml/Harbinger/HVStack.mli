val clear: unit->unit;;
val openFunc: unit->unit;;
val closeFunc: unit->unit;;
val openLvl: unit->unit;;
val closeLvl: unit->unit;;
val getValue:
	HTypes.var -> HTypes.value;;
val update:
	HTypes.var->HTypes.value->unit;;
val create:
	HTypes.varType->string->unit;;
val getVarType:
	HTypes.var -> HTypes.varType;;
val addRef:
	HTypes.var->HTypes.varType->string->unit;;
	
(*type vType=
	Value of HTypes.varType*HTypes.value ref
	| Ref of HTypes.varType*HTypes.value ref*HTypes.var list;;	
	
val stack: (string*vType) list list list ref;;*)
	

