signature SET =
sig
    structure Elem : ORDERED
    type Set

    val empty : Set
    val insert : Elem.T -> Set -> Set
    val member : Elem.T -> Set -> bool
end
