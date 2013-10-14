newtype SSC a = SSC (String -> (a, String))

bind (SSC f) g = SSC (temp)
	where temp state = q nstate
		where
			SSC(q) = g val
			(val, nstate) = f state

combine (SSC f) (SSC g) = SSC(temp)
	where temp state = g nstate
		where (_, nstate) = f state
		
instance Monad SSC where
	(>>=) = bind
	(>>) = combine
	fail _ = undefined
	return a = SSC(\state -> (a, state))
	
isEOS = SSC(\s -> (null s, s))
getc = SSC(\(h:t) -> (h, t))
runSSC (SSC a) s = fst $ a s
ungetc c  = SSC(\s -> ((), c : s))

countLines :: String -> Int
countLines = runSSC $ lines 0 where
   lines :: Int -> SSC Int
   lines n = do
      b <- isEOS
      if b
         then return n
         else do
            ch <- getc
            lines (if ch == '\n' then n+1 else n)
