load "Random";
(*
functor TestHeaps (F : functor (El : ORDERED) -> HEAP) : sig type Q; val test : unit -> Q end =
    struct
        structure Heap = F(OrderedReal)
        type Q = Heap.Heap
*)
structure Heap = LeftistHeap(OrderedReal)

fun add 0 _ h = h
    | add n gen h = add (n - 1) gen (Heap.insert (Random.random gen, h))
fun remove 0 h = h
    | remove n h = remove (n - 1) (Heap.deleteMin h)
fun test () =
    let
        val n = 2000000
        val seed = 13.0
    in
        remove n (add n (Random.newgenseed seed) Heap.empty)
    end

(*
    end
*)
