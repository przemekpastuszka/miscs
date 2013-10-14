module Test where

import List

quicksort [] = []
quicksort [a] = [a]
quicksort (h : t) =
	let (small, big) = partition (< h) t
	in (quicksort small) ++ (h : (quicksort big))
