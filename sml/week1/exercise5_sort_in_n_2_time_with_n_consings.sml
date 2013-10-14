load "Int";

fun sort ls =
    let
        fun min a b = if a < b then a else b

        fun minGreaterThan [] _ acc = acc
        | minGreaterThan (h :: t) lowerBound acc =
            if h > lowerBound
            then
                case acc of
                    NONE => minGreaterThan t lowerBound (SOME h)
                    | SOME x => minGreaterThan t lowerBound (SOME (min x h))
            else
                minGreaterThan t lowerBound acc

        fun sortTmp lowerBound acc =
            case minGreaterThan ls lowerBound NONE of
                NONE => acc
                | SOME x => sortTmp x (x :: acc)
    in
        sortTmp (Option.valOf Int.minInt) []
    end


