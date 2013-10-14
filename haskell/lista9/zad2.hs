fibSum = 
	let fibHelp acc (a, b) =
		if b >= 4000000 then acc
		else if mod b 2 == 0 
		then fibHelp (acc + b) (b, a + b)
		else fibHelp b (b, a + b)
	in fibHelp 0 (1, 1)
