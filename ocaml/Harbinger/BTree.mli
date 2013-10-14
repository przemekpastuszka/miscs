type ('a, 'b) bTree =
    Leaf
  | Node of ('a, 'b) bTree * ('a * 'b) * ('a, 'b) bTree
  
val empty : ('a, 'b) bTree

exception BadComparer

val get :
  ?cmp:('a -> 'a -> int) -> ?modify:('a -> 'a) -> ('a, 'b) bTree -> 'a -> 'b
  
val add :
  ?cmp:('a -> 'a -> int) ->
  ?modify:('a -> 'a) -> ('a, 'b) bTree -> 'a * 'b -> ('a, 'b) bTree
  
val update :
  ?cmp:('a -> 'a -> int) ->
  ?modify:('a -> 'a) -> ('a, 'b) bTree -> 'a * 'b -> ('a, 'b) bTree