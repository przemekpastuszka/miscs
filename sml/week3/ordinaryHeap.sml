functor OrdinaryHeap (Element : ORDERED) : HEAP =
struct
    structure Elem = Element

    datatype Tree = E | Node of Elem.T * Tree * Tree
    type Heap = int * Tree

    exception EmptyHeap

    val empty = (0, E)
    fun isEmpty (_, E) = true
        | isEmpty (_, _) = false

    fun insTree (x, k) E = Node(x, E, E)
        | insTree (x, k) (Node(y, left, right)) =
            let
                val (min, max) = if Elem.leq x y then (x, y) else (y, x)
            in
                if k mod 2 = 0 then
                    Node(min, insTree (max, k div 2) left, right)
                else
                    Node(min, left, insTree (max, k div 2) right)
            end

    fun insert (x, (_, E)) = (1, Node(x, E, E))
        | insert (x, (num, t)) = (num + 1, insTree (x, num + 1) t)

    fun merge a b = a

    fun findMin (_, E) = raise EmptyHeap
        | findMin (_, Node(x, _, _)) = x

    fun removeLast E _ = raise EmptyHeap
        | removeLast (Node(x, E, E)) _ = (x, E)
        | removeLast (Node(x, left, right)) k =
            if k mod 2 = 0 then
                let val (v, t) = removeLast left (k div 2)
                in (v, Node(x, t, right)) end
            else
                let val (v, t) = removeLast right (k div 2)
                in (v, Node(x, left, t)) end


    fun fixTree E = E
        | fixTree (t as Node(x, E, E)) = t
        | fixTree (t as Node(x, Node(a, E, E), E)) =
            if Elem.leq x a then t else Node(a, Node(x, E, E), E)
        | fixTree (t as Node(x, E, Node(a, E, E))) =
            if Elem.leq x a then t else Node(a, E, Node(x, E, E))
        | fixTree (t as Node(x, ta as Node(a, la, ra), tb as Node(b, lb, rb))) =
            if Elem.leq a b then
                if Elem.leq x a then t
                else Node(a, fixTree (Node(x, la, ra)), tb)
            else
                if Elem.leq x b then t
                else Node(b, ta, fixTree (Node(x, lb, rb)))

    fun deleteMin (_, E) = raise EmptyHeap
        | deleteMin (_, Node(x, E, E)) = (0, E)
        | deleteMin (n, t) =
            let val (v, Node(x, l, r)) = removeLast t n
            in (n - 1, fixTree (Node(v, l, r))) end
end
