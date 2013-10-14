datatype tree = Node of int * tree * tree | E

fun cbt 0 _ = E
    | cbt n e =
        let
            val t = cbt (n - 1) e
        in Node(e, t, t)
        end

fun cbtList 0 e = [(0, E)]
    | cbtList n e =
        let
            val ls as (num, h)::_ = cbtList (n - 1) e
        in (2 * num + 1, Node(e, h, h)) :: ls
        end

fun log2 1 = 0
    | log2 n = 1 + log2 (n div 2)

fun bt n e =
    let
        fun bt2 0 _ _  = E
        | bt2 1 e _  = Node(e, E, E)
        | bt2 n e ((num1, h1)::(num2, h2)::t) =
                if n = num1 then h1
                else if n - 1 = num1 + num2 then Node(e, h1, h2)
                else if n - 1 > num1 + num2 then
                    Node(e, h1, bt2 (n - num1 - 1) e ((num2, h2)::t))
                else
                    Node(e, bt2 (n - num2 - 1) e ((num2, h2)::t), h2)
    in bt2 n e (cbtList (log2 (n + 1)) e)
    end

