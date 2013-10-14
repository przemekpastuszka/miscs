functor UnbalancedMap (KEY : ORDERED) : FiniteMap =
struct
    structure Key = KEY

    structure 'a OrderedPair =
        struct
            type T = Key.T *  Q
            fun leq (k1, _) (k2, _) = Key.leq k1 k2
        end : ORDERED
        withtype  Q = 'a

    type 'a Map = UnbalancedSet(OrderedPair 'a).Set



    val empty = Map.empty
    fun bind p m = Map.insert p m
(*
    val lookup : Key.T -> 'a Map -> 'a
*)

end
