newtype Random a = Random (Int -> (a, Int))

initR seed = Random(\_ -> ((), seed))
random = Random(temp)
	where temp seed = (newSeed, newSeed)
		where
			q = 16807 * (mod seed 127773) - 2836 * (div seed 127773)
			newSeed
				| q > 0 = q
				| otherwise = q + 2147483647

bind (Random f) g = Random (temp)
	where temp seed = q nseed
		where
			Random(q) = g val
			(val, nseed) = f seed

combine (Random f) (Random g) = Random(temp)
	where temp seed = g nseed
		where (_, nseed) = f seed
		
instance Monad Random where
	(>>=) = bind
	(>>) = combine
	fail _ = undefined
	return a = Random(\seed -> (a, seed))
	
deRandom seed (Random f) = fst $ f seed

example = deRandom 3 (do
x <- random
y <- random
z <- random
w <- random
initR 5
q <- random
return (x, y, z, w, q))
