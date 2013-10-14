multiply ls = 
	case foldr (\n p -> p >>= (q n)) (return 1) ls of
		Nothing -> 0
		Just x -> x
	where
		q 0 _ = Nothing
		q n p = return $ n * p
