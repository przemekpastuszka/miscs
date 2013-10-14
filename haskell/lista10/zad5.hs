module Test where

insert a [] = [a]
insert a (h : t) =
	if a < h then a : h : t
	else h : (insert a t)
	
insertionSort [] = []
insertionSort (h : t) =
	insert h (insertionSort t)
