signature SET =
sig
    type Elem
    type Set

    val empty : Set
    val singleton : Elem -> Set

    val toList : Set -> Elem list
(*
    val fromList : Elem list -> Set
*)
    val fromOrdList : Elem list -> Set
(*
    val size : Set -> int
    val member : Elem * Set -> bool
*)
    val add : Elem * Set -> Set
(*
    val delete : Elem * Set

    val union : Set * Set -> Set
    val intersection : Set * Set -> Set
    val difference : Set * Set -> Set
*)
end
