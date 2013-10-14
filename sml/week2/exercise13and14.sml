datatype Heap = E | T of int * int * Heap * Heap

fun rank E = 0
    | rank (T(r, _, _, _)) = r

fun makeT x left right =
    if rank left < rank right then
        T(rank left + 1, x, right, left)
    else
        T(rank right + 1, x, left, right)

fun insert x E = T(1, x, E, E)
    | insert x (T(_, y, left, right)) =
        if x < y then
            makeT x left (insert y right)
        else
            makeT y left (insert x right)

fun merge h E = h
    | merge E h = h
    | merge (h1 as T(_, x, a1, b1)) (h2 as T(_, y, a2, b2)) =
        if x < y then
            makeT x a1 (merge b1 h2)
        else
            makeT y a2 (merge h1 b2)

fun
    fromList ls =
    let
        fun mergeStep [] = []
        | mergeStep [x] = [x]
        | mergeStep (a::b::c) =
            merge a b :: mergeStep c

        fun fromHeapList [] = E
        | fromHeapList [x] = x
        | fromHeapList (l as a::b::c) =
            fromHeapList (mergeStep l)

    in fromHeapList (map (fn x => T(1, x, E, E)) ls)
    end
