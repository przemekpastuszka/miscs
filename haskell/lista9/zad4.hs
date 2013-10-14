fac 0 = 1
fac n = n * fac(n - 1)

facSum =
	let helper current acc =
		if current == 0 then acc
		else helper (div current 10) (mod current 10 + acc)
	in helper (fac 100) 0
