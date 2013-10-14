fib 0 = 1
fib n = 
	let fibHelp x (a, b) =
		if x == n then b
		else fibHelp (x + 1) (b, a + b)
	in fibHelp 1 (1, 1)
