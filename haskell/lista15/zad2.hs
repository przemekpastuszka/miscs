data Term sig var = Var var | FunSym sig [Term sig var]

{--compute (FunSym f ls) = f $ map (\x -> compute x) ls
compute (Var x) = x

bind (FunSym f ls) b = b $ f $ map (\x -> compute x) ls
bind (Var x) b = b x

instance Monad (Term sig) where
	(>>=) = bind
	_ >> _ = undefined
	fail _ = undefined
	return a = Var a--}
	
instance Monad (Term sig) where
	(Var a) (>>=) b = b a
	
	_ >> _ = undefined
	fail _ = undefined
	return a = 
