newtype StateComput s a = SC (s -> (a,s))

bind (SC f) g = SC (temp)
	where temp state = q nstate
		where
			SC(q) = g val
			(val, nstate) = f state

combine (SC f) (SC g) = SC(temp)
	where temp state = g nstate
		where (_, nstate) = f state
		
instance Monad (StateComput a) where
	(>>=) = bind
	(>>) = combine
	fail _ = undefined
	return a = SC(\state -> (a, state))
