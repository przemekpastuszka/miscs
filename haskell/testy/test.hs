f = do
	x <- [0..3]
	y <- drop 1 [0..x]
	return (x, y)
