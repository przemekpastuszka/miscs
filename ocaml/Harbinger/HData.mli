val functions: ((string*HTypes.varType list)*HTypes.funcType) list ref
val structs: (string*HTypes.hStruct) list ref

val get: 'a -> ('a*'b) list ref -> 'b
val remove: 'a -> ('a*'b) list ref -> unit
val add: ('a*'b) -> ('a*'b) list ref -> unit
val update:  ('a*'b) -> ('a*'b) list ref -> unit
val clear: 'a list ref -> unit