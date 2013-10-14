import Control.Monad;

newtype Parser token m value = Parser ([token] -> m ([token],value))

bind (Parser f) g = Parser(temp)
	where temp ls = do
		(nls, value) <- f ls
		let (Parser q) = g value in
			q nls
			
combine (Parser f) (Parser g) = Parser(temp)
	where temp ls = do
		(nls, _) <- f ls
		g nls
		
instance Monad m => Monad (Parser token m) where
	(>>=) = bind
	(>>) = combine
	fail _ = undefined
	return a = Parser(\x -> return (x, a))
	
parse (Parser f) ls = do
	(rest, value) <- f ls
	guard $ null rest
	return value
	
instance MonadPlus m => MonadPlus(Parser token m) where
	mplus (Parser f) (Parser g) = Parser(\x -> mplus (f x) (g x))
	mzero = Parser (\x -> mzero)
	
isElem ls = Parser(temp)
	where
		temp [] = mzero
		temp (a : b)
			| elem a ls = return (b, a)
			| otherwise = mzero

isEmpty :: MonadPlus m => Parser token m ()			
isEmpty = Parser(\x -> return (x, ()));

many (Parser p) = Parser(f [])
	where f vals ls =
		let m = p ls in
			if m == mzero then
				return (ls, reverse vals)
			else
				m >>= (\(ntokens, v) -> f (v : vals) ntokens)
many1 p = Parser(temp)
	where
		(Parser m) = many p
		temp ls = do
			(tokens, vals) <- m ls
			guard $ not $ null vals
			return (tokens, vals)

tmp (Parser f) ls = f ls

intNumber :: Parser Char [] Double
intNumber = do
	x <- many1 $ isElem "1234567890"
	return $ readDouble x
	
realNumber :: Parser Char [] Double
realNumber = mplus intNumber (do
	x <- intNumber
	isElem ".,"
	y <- intNumber
	return $ readDouble ((show x) ++ "." ++ (show y)))

readDouble :: String -> Double
readDouble = read
	
addOrMinusTmp = do
	x <- mulOrDivide
	tmp x
	where tmp acc = mplus (return acc) (do
		op <- isElem "+-"
		y <- mulOrDivide
		case op of
			'+' -> tmp (acc + y)
			'-' -> tmp (acc - y))

mulOrDivideTmp = do
	x <- realNumber
	tmp x
	where tmp acc = mplus (return acc) (do
		op <- isElem "*/"
		y <- realNumber
		case op of
			'*' -> tmp (acc * y)
			'/' -> tmp (acc / y))			

mulOrDivide = mplus realNumber mulOrDivideTmp
