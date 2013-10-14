import Data.List

--a)
insertEverywhere left el [] = [x]
	where x = reverse $ el : left
insertEverywhere left el (h : t) = 
	(left ++ (el : h : t)) : insertEverywhere (h : left) el t

perm [] = [[]]
perm (h : t) = concatMap (insertEverywhere [] h) (perm t)


--b)
perm2 [] = [[]]
perm2 (h : t) = [ xs | p <- perm2 t, xs <- insertEverywhere [] h p]

--c)
perm3 [] = return []
perm3 (h : t) =
	do
		p <- perm3 t
		xs <- insertEverywhere [] h p
		return xs
