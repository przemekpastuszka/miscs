module Test where

getRidOf a [] = []
getRidOf a (h : t) =
	if a == h then t
	else h : (getRidOf a t)

{--getMin (h : t) =
	let
		helper el [] = el
		helper el (h : t) = helper (min el h) t
	in helper h t--}

getMin (h : t) = foldl min h t

selectionSort [] = []
selectionSort ls =
	let min = getMin ls
	in min : (selectionSort (getRidOf min ls))
