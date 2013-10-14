data Vars = A | B
data Fun a = AND (Fun a) (Fun a) | OR (Fun a) (Fun a) | NOT (Fun a) | NOR (Fun a) (Fun a) | NAND (Fun a) (Fun a) | Var a

compute (AND a b) env = (compute a env) * (compute b env)
compute (NOT a) env = 1 - (compute a env)
compute (NAND a b) env = 1 - (compute (AND a b) env)
compute (NOR a b) env = 1 - (compute (OR a b) env)
--compute (Var a) [] = raise "Pieprzony dziadu"
compute (Var v) ((a, val) : t) =
    if v == a then val
    else compute (Var v) t
compute (OR a b) env =
    if (compute a env) + (compute b env) > 0 then 1
    else 0

fuck :: Fun Int -> Fun Int -> Int
fuck a b =
    let
        x = map (compute a) [[(0, 0), (1, 0)], [(0, 0), (1, 1)], [(0, 1), (1, 0)], [(0, 0), (1, 1)]]
        y = map (compute b) [[(0, 0), (1, 0)], [(0, 0), (1, 1)], [(0, 1), (1, 0)], [(0, 0), (1, 1)]]
    in if x == y then 1 else 0
