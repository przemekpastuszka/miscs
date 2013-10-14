module Test where

halve ls =
	let
		halveH x [] = ([], x)
		halveH x [a] =  ([], x)
		halveH (h : t) (a : b : c) =
			let (x, y) = halveH t c
			in (h : x, y)
	in halveH ls ls
	
merge [] ls2 = ls2
merge ls [] = ls
merge (a : b) (x : y) =
	if a < x then a : merge b (x : y)
	else x : merge (a : b) y

mergeSort [] = []
mergeSort [a] = [a]
mergeSort ls = 
	let (left, right) = halve ls
	in merge (mergeSort left) (mergeSort right)
	
