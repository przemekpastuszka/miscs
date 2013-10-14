class Monoid a where
   (***) :: a -> a -> a
   e :: a
infixl 6 ***

infixr 7 ^^^
(^^^) :: Monoid a => a -> Integer -> a
a ^^^ 0 = e
a ^^^ n
	| odd n = a *** tmp *** tmp
	| even n = tmp *** tmp
	where tmp = a ^^^ (div n 2)
	
data Mtx2x2 a = Mtx2x2 a a a a

instance Num a => Monoid (Mtx2x2 a) where
	e = Mtx2x2 0 1 1 0
	(***) (Mtx2x2 a11 a12 a21 a22) (Mtx2x2 b11 b12 b21 b22) =
		Mtx2x2
			(a11 * b11 + a12 * b21)
			(a11 * b12 + a12 * b22)
			(a21 * b11 + a22 * b21)
			(a21 * b12 + a22 * b22)
			
fib 0 = 0
fib n = a12
	where (Mtx2x2 _ a12 _ _) = (Mtx2x2 0 1 1 1) ^^^ n
