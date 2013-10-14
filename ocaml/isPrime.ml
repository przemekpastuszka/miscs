let isPrime x =
	let rec isDivisor y =
		if y = 1 then true
		else if x mod y = 0 then false
		else isDivisor (y - 1)
	in isDivisor (x / 2);;
