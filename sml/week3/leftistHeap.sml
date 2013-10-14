functor LeftistHeap (Element : ORDERED) : HEAP =
struct
    structure Elem = Element
    datatype Heap = E | T of int * Elem.T * Heap * Heap

    val empty = E
    fun isEmpty E = true
        | isEmpty _ = false

    fun rank E = 0
        | rank (T(r, _, _, _)) = r

    fun makeT x left right =
        if rank left < rank right then
            T(rank left + 1, x, right, left)
        else
            T(rank right + 1, x, left, right)

    fun merge h E = h
    | merge E h = h
    | merge (h1 as T(_, x, a1, b1)) (h2 as T(_, y, a2, b2)) =
        if Elem.leq x y then
            makeT x a1 (merge b1 h2)
        else
            makeT y a2 (merge h1 b2)

    fun insert (x, E) = T(1, x, E, E)
        | insert (x, T(_, y, left, right)) =
            if Elem.leq x y then
                makeT x left (insert (y, right))
            else
                makeT y left (insert (x, right))

    exception EmptyHeap

    fun findMin E = raise EmptyHeap
        | findMin (T(_, x, _, _)) = x

    fun deleteMin E = raise EmptyHeap
        | deleteMin (T(_, _, left, right)) = merge left right
end
