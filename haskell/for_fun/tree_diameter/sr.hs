import Data.List

--drzewo skierowane
data Tree = Node [Tree]

listMax = foldl max 0

compute (Node []) = (0, 0)
compute (Node [a]) =
	let (dist, m) = compute a
	in (dist + 1, max (dist + 1) m)
compute (Node ls) =
	let
		(dists, maxs) = unzip $ map compute ls
		m1 = listMax dists
		m2 = listMax $ delete m1 dists
	in
		(m1 + 1, listMax $ m1 + m2 + 2 : maxs)

diameter = snd.compute

t = Node [
	Node [],
	Node [],
	Node [Node [Node [Node []]], Node [Node [Node []]]],
	Node [Node [Node [Node []]], Node [Node [Node []]]]]
