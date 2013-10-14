module Test where

newtype FSet a = FSet (a -> Bool)

empty :: FSet a
empty = FSet (\x -> False)

singleton :: Ord a => a -> FSet a
singleton a = FSet (\x -> a == x)

fromList :: Ord a => [a] -> FSet a
fromList ls = foldl (\(FSet x) el -> FSet (\z -> if z == el then True else x z)) empty ls

union :: Ord a => FSet a -> FSet a -> FSet a
union (FSet a) (FSet b) = FSet (\x -> (a x) || (b x))

intersection :: Ord a => FSet a -> FSet a -> FSet a
intersection (FSet a) (FSet b) = FSet (\x -> (a x) && (b x))

member :: Ord a => a -> FSet a -> Bool
member x (FSet a) = a x
