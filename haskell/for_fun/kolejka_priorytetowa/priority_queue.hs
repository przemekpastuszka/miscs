import Data.Maybe

data BTree a = Leaf | Node (BTree a) a (BTree a)

minimum :: BTree a -> Maybe a
minimum Leaf = Nothing
minimum (Node _ a _ ) = Just a

--cóś nie działa
--ogólnie pisanie takiej rzeczy w Haskellu to większy syf niż przewiduje ustawa
extractMin :: Ord a => BTree a -> BTree a
extractMin Leaf = Leaf
extractMin (Node Leaf _ Leaf) = Leaf
extractMin root@(Node left _ right) =
	normalize (Node nleft leaf nright)
	where
		((Node nleft _ nright), leaf) = extractLeaf root
		extractLeaf (Node Leaf leaf Leaf) = (Leaf, leaf)
		extractLeaf (Node left x Leaf) = (Node newNode x Leaf, leaf)
			where (newNode, leaf) = extractLeaf left
		extractLeaf (Node left x right) = (Node left x newNode, leaf)
			where(newNode, leaf) = extractLeaf right
		normalize Leaf = Leaf
		normalize node@(Node Leaf _ Leaf) = node
		normalize node@(Node Leaf x (Node l y r)) =
			if(x > y) then Node Leaf y (normalize (Node l x r))
			else node
		normalize (Node a x Leaf) = normalize (Node Leaf x a)
		normalize node@(Node left@(Node l1 l l2) x right@(Node r1 r r2)) =
			if x <= l && x <= r then node
			else if l < r then Node (normalize (Node l1 x l2)) l right
			else Node left r (normalize (Node r1 x r2))
	
--testing queen
t = Node 
	(Node (Node (Node Leaf 31 Leaf) 17 Leaf) 13 (Node Leaf 14 Leaf)) 9
	(Node (Node Leaf 35 Leaf) 21 (Node Leaf 22 Leaf))

--insert :: Eq a => a -> BTree a -> BTree a
