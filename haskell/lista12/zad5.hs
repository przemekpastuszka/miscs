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

{--
instance Monoid Integer where
	(***) = \a b -> mod (a * b) 9876543210
	e = 1

1234567890 ^^^ 1234567890 = 5323470030
--}


instance Monoid Integer where
	(***) = (+)
	e = 0

