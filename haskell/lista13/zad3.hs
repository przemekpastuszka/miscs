--a)
sublists [] = [[]]
sublists (h : t) = (map (h:) res) ++ res
	where res = sublists t

--b)
sublists2 [] = [[]]
sublists2 (h : t) = [ h : xs | xs <- res] ++ res
	where res = sublists2 t 
	
--c)

sublists3 [] = return []
sublists3 (h : t) = gen ++ res
	where
	res = sublists3 t
	gen = do
		xs <- res
		return $ h : xs
