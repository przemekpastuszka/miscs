class Set a where
   empty :: a
   isEmpty :: a -> Bool
   singleton :: Integer -> a
   fromList :: [Integer] -> a
   union :: a -> a -> a
   intersection :: a -> a -> a
   member :: Integer -> a -> Bool

newtype ListSet = ListSet [Integer]

instance Set ListSet where
	empty = ListSet []
	isEmpty (ListSet []) = True
	isEmpty (ListSet _) = False
	singleton a = ListSet [a]
	fromList = ListSet
	union (ListSet a) (ListSet b) = ListSet (a ++ b)
	intersection a (ListSet b) = ListSet (filter ((flip member) a) b)
	member a (ListSet ls) = elem a ls
	
data TreeSet = Node TreeSet Integer TreeSet | Leaf

insert Leaf a = Node Leaf a Leaf
insert (Node left key right) a
	| a < key = Node (insert left a) key right
	| a > key = Node left key (insert right a)
	| otherwise = Node left key right

flatten t =
	let
		flattenH Leaf acc = acc
		flattenH (Node left k right) acc =
			flattenH right (k : flattenH left acc)
	in reverse (flattenH t [])

intersect [] _ = []
intersect _ [] = []
intersect ls@(a : b) ls2@(h : t)
	| a < h = intersect b ls2
	| a > h = intersect ls t
	| otherwise = a : intersect b t

instance Set TreeSet where
	empty = Leaf
	isEmpty Leaf = True
	isEmpty (Node _ _ _) = False
	singleton = insert Leaf
	fromList = foldl insert Leaf
	union a b = foldl insert a (flatten b)
	intersection a b = fromList (intersect (flatten a) (flatten b))
	member a Leaf = False
	member a (Node left key right)
		| a < key = member a left
		| a > key = member a right
		| otherwise = True
