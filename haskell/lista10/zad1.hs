module Test where
root (a, b, c) =
	let det = sqrt(b * b - 4 * a * c)
	in case compare 0 det of
		LT -> [(-b - det)/2*a, (-b + det)/2*a]
		EQ -> [-b/2*a]
		GT -> []
