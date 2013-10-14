fun divideHalfTmp [] left right = (left, right)
    | divideHalfTmp [x] left right = (left, right)
    | divideHalfTmp (a :: b :: c) left (h :: t) =
        divideHalfTmp c (h :: left) t

fun divideHalf ls = divideHalfTmp ls [] ls

fun merge [] a = a
    | merge a [] = a
    | merge (h :: t) (b :: c) =
        if h < b then
            h :: merge t (b :: c)
        else
            b :: merge (h :: t) c

fun mergeSort [] = []
    | mergeSort [x] = [x]
    | mergeSort (a :: b :: c) =
        let
            val (left, right) = divideHalf c
            val leftSorted = mergeSort (a :: left)
            val rightSorted = mergeSort (b :: right)
        in
            merge leftSorted rightSorted
        end
