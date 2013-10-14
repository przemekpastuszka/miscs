merge ls [] n = ls
merge [] ls n = ls
merge ls ls2 0 = ls2
merge (a : b) (x : y) n =
	if a < x then a : merge b (x : y) (n - 1)
	else x : merge (a : b) y n

mergeSort ls =
	let
		mergeSortH [] n = []
		mergeSortH (a : t) 1 = [a]
		mergeSortH ls 0 = []
		mergeSortH ls n =
			let
				len = div n 2
				left = mergeSortH ls len
				right = mergeSortH (drop len ls) (n - len)
			in merge left right len
	in mergeSortH ls (length ls)
