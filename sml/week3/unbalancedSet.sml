functor UnbalancedSet(Element : ORDERED) : SET =
struct
    structure Elem = Element
    datatype Set = E | T of Elem.T * Set * Set

    val empty = E
    fun insert x E = T(x, E, E)
        | insert x (T(y, left, right)) =
            if Elem.leq x y then T(y, insert x left, right)
            else T(y, left, insert x right)

    fun member x E = false
        | member x (T(y, left, right)) =
            if Elem.leq x y then
                if Elem.leq y x then true
                else member x left
            else member x right
end
