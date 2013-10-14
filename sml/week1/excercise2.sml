fun partition [] pivot acc = acc
    | partition (h :: t) pivot (left, right) =
        if h < pivot then
            partition t pivot (h :: left, right)
        else
            partition t pivot (left, h :: right)

fun qsortAcc [] acc = acc
    | qsortAcc (h :: t) acc =
        let
            val (left, right) = partition t h ([], [])
        in
            qsortAcc left (h :: (qsortAcc right acc))
        end

fun qsort ls = qsortAcc ls []


fun partition2 [] pivot = ([], [])
    | partition2 (h :: t) pivot =
        let val (left, right) = partition2 t pivot
        in
            if h < pivot then (h :: left, right)
            else (left, h :: right)
        end
