import Control.Monad

setHetman col n ls =
	do
	row <- [1..n]
	guard $ null $ filter (\ (c,r) -> r == row || abs (col - c) == abs (row - r)) ls
	return $ (col, row) : ls

hetman n = solve n n
	where	
	solve 0 _ = return []
	solve col n =
		do
		prev <- solve (col - 1) n
		setHetman col n prev
