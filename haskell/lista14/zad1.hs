data Cyclist a = Elem (Cyclist a) a (Cyclist a)

forward (Elem _ _ x) = x
backward (Elem x _ _) = x
label (Elem _ x _) = x

--fromList l@(h : t) = Elem (fromList $ [last l] ++ (take ((length l) - 1) l)) h (fromList $ t ++ [h])

fromList [] = undefined 
fromList l =
	fromListH l r l r where
		r = reverse l
		fromListH orgL orgLR [] r = fromListH orgL orgLR orgL r
		fromListH orgL orgLR l [] = fromListH orgL orgLR l orgLR
		fromListH orgL orgLR l@(h : t) r@(hr : tr) = Elem b h f
			where
				b = fromListH orgL orgLR  (hr : l) tr
				f = fromListH orgL orgLR  t (h : r)
