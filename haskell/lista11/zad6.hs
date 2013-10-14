module Test where

data Tree a = Node (Tree a) a (Tree a) | Leaf

insert Leaf a = Node Leaf a Leaf
insert (Node left key right) a =
	if a < key then Node (insert left a) key right
	else Node left key (insert right a)

flatten t =
	let
		flattenH Leaf acc = acc
		flattenH (Node left k right) acc =
			flattenH right (k : flattenH left acc)
	in reverse (flattenH t [])
	
treeSort ls =
	flatten (foldl insert Leaf ls)
