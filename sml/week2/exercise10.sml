datatype tree = Node of int * tree * tree | E

fun insert x t =
    let
        fun insert2 x E el = if x = el then E else Node(x, E, E)
        | insert2 x (t as Node(y, left, right)) el =
            if x <= y then
                case insert2 x left y of
                    E => E
                    | nt as Node(_, _, _) => Node(y, nt, right)
                else case insert2 x right el of
                    E => E
                    | nt as Node(_, _, _) => Node(y, left, nt)
    in case insert2 x t (x + 1) of
        E => t
        | nt as Node(_, _, _) => nt
    end
