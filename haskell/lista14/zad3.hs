data Cyclist a = Elem (Cyclist a) a (Cyclist a)
newtype Cyc a b = Cyc (Cyclist a -> (b, Cyclist a))

forward (Elem _ _ x) = x
backward (Elem x _ _) = x
label (Elem _ x _) = x


enumInts = cycleListFrom 0
	where cycleListFrom x = Elem (cycleListFrom (x - 1)) x (cycleListFrom (x + 1))
	
runCyc x (Cyc f) = fst $ f x
fwd = Cyc (\a -> ((), forward a))
bkw = Cyc (\a -> ((), backward a))
lbl = Cyc (\a -> (label a, a))

bind (Cyc f) g = Cyc(temp)
	where temp state = q nstate
		where 
			Cyc(q) = g val
			(val, nstate) = f state

combine (Cyc f) (Cyc g) = Cyc(temp)
	where temp state = g nstate
		where (_, nstate) = f state

instance Monad (Cyc a) where
	(>>=) = bind
	(>>) = combine
	return a = Cyc (\state -> (a, state))
	fail _ = undefined
	
example = runCyc enumInts (do
   bkw
   bkw
   bkw
   bkw
   x <- lbl
   fwd
   fwd
   y <- lbl
   fwd
   z <- lbl
   return (x+y+z))
