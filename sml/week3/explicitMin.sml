functor ExplicitMin(H : HEAP) : HEAP =
struct
    structure Elem = H.Elem
    datatype Heap = E | NE of Elem.T * H.Heap

    val empty = E
    fun isEmpty E = true
        | isEmpty _ = false

    fun min x y = if Elem.leq x y then x else y

    fun merge E h = h
        | merge h E = h
        | merge (NE(m1, h1)) (NE(m2, h2)) =
            NE(min m1 m2, H.merge h1 h2)

    fun insert (x, E) = NE(x, H.insert (x, H.empty))
        | insert (x, NE(y, h)) = NE(min x y, H.insert (x, h))

    exception EmptyHeap

    fun findMin E = raise EmptyHeap
        | findMin (NE(m, _)) = m

    fun deleteMin E = raise EmptyHeap
        | deleteMin (NE(_, h)) =
            let
                val newH = H.deleteMin h
            in
                NE(H.findMin newH, newH) handle H.EmptyHeap => E
            end
end
