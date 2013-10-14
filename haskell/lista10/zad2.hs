module Test where
data Roots = No | One Double | Two (Double, Double) deriving Show
root (a, b, c) =
	let det = sqrt(b * b - 4 * a * c)
	in case compare 0 det of
		LT -> Two ((-b - det) / 2 * a, (-b + det) / 2 * a)
		EQ -> One (-b / 2 * a)
		GT -> No
