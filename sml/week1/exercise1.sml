fun rev ls =
    let
        fun revAcc [] acc = acc
        | revAcc (h :: t) acc = revAcc t (h :: acc)
    in
        revAcc ls []
    end

datatype tree = Tip | Node of tree * int * tree

fun flatten t =
    let
        fun flattenAcc Tip acc = acc
        | flattenAcc (Node (l, x, r)) acc =
            flattenAcc l (x :: (flattenAcc r acc))
    in flattenAcc t []
    end

fun sublist (x::xs) =
    let
        fun addx (ys::yss) acc = addx yss ((x:: ys) :: acc)
            | addx [] acc = acc
        val xss = sublist xs
    in
        addx xss xss
    end
    | sublist [] = [[]]
