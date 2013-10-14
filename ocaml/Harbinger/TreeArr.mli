type 'a treeArray = int * (int, 'a) BTree.bTree
exception IndexOutOfBounds
val empty :unit-> int * ('a, 'b) BTree.bTree
val get : int * (int, 'a) BTree.bTree -> int -> 'a
val update :
  int * (int, 'a) BTree.bTree ->
           int * 'a -> 'a -> int * (int, 'a) BTree.bTree