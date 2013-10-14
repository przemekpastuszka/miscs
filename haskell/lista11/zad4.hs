module Test where

merge [] ls2 = ls2
merge ls [] = ls
merge (a : b) (x : y) =
	case compare a x of
		LT -> a : merge b (x : y)
		EQ -> a : merge b y
		GT -> x : merge (a : b) y
		
d235 =
	let gen x = x : merge (merge (gen (2 * x)) (gen (3 * x))) (gen (5 * x))
	in gen 1
