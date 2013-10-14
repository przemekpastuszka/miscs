data Cyclist a = Elem (Cyclist a) a (Cyclist a)

forward (Elem _ _ x) = x
backward (Elem x _ _) = x
label (Elem _ x _) = x

enumInts = cycleListFrom 0
	where cycleListFrom x = Elem (cycleListFrom (x - 1)) x (cycleListFrom (x + 1))
