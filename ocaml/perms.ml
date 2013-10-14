let fromCycle ls n=
	let rec helper l first =
		match l with
		[h] -> [(h, first)]
		| a::b::t -> (a,b)::(helper (b::t) first)
	in
	List.flatten (List.map (fun (h::t) -> helper (h::t) h) ls);; 


