mtails :: [a] -> [[a]]

--a)

mtails [] = [[]]
mtails r@(_:t) = r : mtails t

--b)

mtails2 ls = [ drop n ls | n <- [0..length ls]]

--c)

mtails3 ls = do
	n <- [0..length ls]
	return $ drop n ls
