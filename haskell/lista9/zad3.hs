kmin n =
	let kminHelp k last =
		if mod last 2 == 0 then
			if last == 2 then k
			else  kminHelp (k + 1) (div last 2)
		else kminHelp (k + 1) (3 * last + 1)
	in kminHelp 1 n
	
kmax =
	let kmaxHelp n max =
		if n == 10000 then max
		else
			let value = kmin n
		in if value > max then kmaxHelp (n + 1) value
		else kmaxHelp (n + 1) value
	in kmaxHelp 1 0
