signature FiniteMap =
sig
    structure Key : ORDERED
    type 'a Map

    val empty : 'a Map
    val bind : Key.T * 'a -> 'a Map -> 'a Map
(*
    val lookup : Key.T -> 'Map -> Value.T
*)
end
