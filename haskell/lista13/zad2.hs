import Data.List

--a)
getEl left [] = []
getEl left (h : t) = 
	(h, left ++ t) : getEl (h : left) t

perm [] = [[]]
perm ls = concatMap (\ (h, t) -> map (h:) (perm t)) (getEl [] ls)


--b)
perm2 [] = [[]]
perm2 ls = [x : xs | x <- ls, xs <- perm2 (delete x ls)]

--c)
perm3 [] = return []
perm3 ls =
	do
	x <- ls
	xs <- perm3 (delete x ls)
	return $ x : xs

