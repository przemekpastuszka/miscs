my_length = foldr (\x -> (1 +)) 0
my_length2 = foldl (\a b -> a + 1) 0
con = flip $ foldr (:)
con2 = foldr (++) []
rev = foldl (flip (:)) []
my_sum = foldl (+) 0
