(*
structure T = OrdinaryHeap(OrderedInt)

val x = foldl T.insert T.empty [3, 21, 9, 18, 7, 41, 0, 15, 24, 15, 10, 1]
val y = foldl T.insert T.empty [7, 5, 88, 23, 4, 11]

fun toList t =
    if T.isEmpty t then []
    else (T.findMin t) :: toList (T.deleteMin t)
*)

test ();
quit ();
