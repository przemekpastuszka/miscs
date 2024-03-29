signature HEAP =
sig
    structure Elem : ORDERED
    exception EmptyHeap
    type Heap

    val empty : Heap
    val isEmpty : Heap -> bool

    val insert : Elem.T * Heap -> Heap
    val merge: Heap -> Heap -> Heap

    val findMin : Heap -> Elem.T
    val deleteMin: Heap -> Heap
end
