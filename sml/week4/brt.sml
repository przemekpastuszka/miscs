functor BlackRedTree(Element : ORDERED) : SET =
struct
    type Elem = Element.T
    datatype Color = R | B
    datatype Tree = E | T of Color * Tree * Elem * Tree
    type Set = int * Tree

    val empty = (0, E)
    fun singleton e = (1, T(B, E, e, E))

    fun balance (B, T(R, T(R, a, x, b), y, c), z, d) = T(R, T (B, a, x, b), y, T(B, c, z, d))
        | balance (B, T(R, a, x, T(R, b, y, c)), z, d) = T(R, T (B, a, x, b), y, T(B, c, z, d))
        | balance (B, a, x, T(R, T(R, b, y, c), z, d)) = T(R, T (B, a, x, b), y, T(B, c, z, d))
        | balance (B, a, x, T(R, b, y, T(R, c, z, d))) = T(R, T (B, a, x, b), y, T(B, c, z, d))
        | balance body = T body

    fun computeHeight (oldH, E) = (0, E)
        | computeHeight (oldH, t as T(B, _, _, _)) = (oldH, t)
        | computeHeight (oldH, t as T(R, a, x, b)) = (oldH + 1, T(B, a, x, b))

    fun add (x, (h, s)) =
        let fun ins E = T(R, E, x, E)
            | ins (s as T(color, a, y, b)) =
                if Element.leq x y then balance (color, ins a, y, b)
                else balance(color, a, y, ins b)
            val t = ins s
        in computeHeight (h, t)
        end

    fun getBiggest (T(_, _, x, E)) = x
        | getBiggest (T(_, _, _, r)) = getBiggest r

    fun mergeDisjointA (l as T(B, _, _, _), x, r, 0) = T(R, l, x, r)
        | mergeDisjointA (T(B, l1, y, r1), x, r, n) =
            balance (B, l1, y, mergeDisjointA (r1, x, r, n - 1))
        | mergeDisjointA (T(R, l1, y, r1), x, r, n) =
            balance (R, l1, y, mergeDisjointA (r1, x, r, n))

    fun mergeDisjointB (l as T(B, _, _, _), x, r, 0) = T(R, r, x, l)
        | mergeDisjointB (T(B, l1, y, r1), x, r, n) =
            balance (B, mergeDisjointB (l1, x, r, n - 1), y, r1)
        | mergeDisjointB (T(R, l1, y, r1), x, r, n) =
            balance (R,  mergeDisjointB (l1, x, r, n), y, r1)

    fun unionOrd ((hl, l), (hr, r)) =
        if hl > hr then
            computeHeight (hl, mergeDisjointA (l, getBiggest l, r, hl - hr))
        else
            computeHeight (hr, mergeDisjointB (r, getBiggest l, l, hr - hl))

    fun fromOrdList ls =
        let
            fun mergeStep [] = []
            | mergeStep [x] = [x]
            | mergeStep (a::b::c) =
                unionOrd (a, b) :: mergeStep c

            fun fromTreeList [] = (0, E)
            | fromTreeList [x] = x
            | fromTreeList (l as a::b::c) =
                fromTreeList (mergeStep l)

        in fromTreeList (map (fn x => singleton x) ls)
        end

    fun toList (_, t) =
        let
            fun flatten E acc = acc
            | flatten (T (_, l, x, r)) acc =
                flatten l (x :: (flatten r acc))
        in flatten t []
    end

    fun union (a, b) = a
end
