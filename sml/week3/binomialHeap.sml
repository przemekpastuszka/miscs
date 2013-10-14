functor BinomialHeap (Element : ORDERED) : HEAP =
struct
    structure Elem = Element

    datatype Tree = Node of Elem.T * Tree list
    type Heap = (int * Tree) list

    val empty = []
    fun isEmpty [] = true
        | isEmpty _ = false

    fun link (t1 as Node(x, ls1)) (t2 as Node(y, ls2)) =
        if Elem.leq x y then Node(x, t2 :: ls1)
        else Node(y, t1 :: ls2)

    fun insTree (r,t) [] = [(r,t)]
        | insTree (r, t1) (heap as (rank, t2) :: tail) =
            if r = rank then insTree (r + 1, link t1 t2) tail
            else (r, t1) :: heap

    fun merge h [] = h
        | merge [] h = h
        | merge (left as (r1, t1) :: h1) (right as (r2, t2) :: h2) =
            if r1 < r2 then (r1, t1) :: merge h1 right
            else if r1 > r2 then (r2, t2) :: merge h2 left
            else insTree (r1 + 1, (link t1 t2)) (merge h1 h2)

    fun insert (x, h) = insTree (0, Node(x, [])) h

    exception EmptyHeap

    fun value (Node(x, _)) = x

    fun removeMinTree [] = raise EmptyHeap
        | removeMinTree [(_, t)] = (t, [])
        | removeMinTree ((r, t) :: h) =
            let val (tMin, hLeft) = removeMinTree h
            in
                if Elem.leq (value tMin) (value t) then (tMin, (r, t) :: hLeft)
                else (t, h)
            end

    fun findMin [] = raise EmptyHeap
        | findMin [(_, t)] = value t
        | findMin ((_, t) :: h) =
            let val min = findMin h in
                if Elem.leq min (value t) then min else value t
            end

    fun fillWithRanges [] r = []
        | fillWithRanges (h :: t) r = (r, h) :: fillWithRanges t (r + 1)

    fun deleteMin h =
        let val (Node(_, ls), heap) = removeMinTree h in
            merge heap (fillWithRanges (rev ls) 0)
        end
end
