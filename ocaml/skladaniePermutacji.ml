let rec zloz perm1 perm2=
	match perm1 with
	[] -> []
	| (nr, cel)::t -> 
		(nr, List.assoc cel perm2)::zloz t perm2;;

let zrob ls =
	let rec pomocnik l=
	match l with
	h::t -> 
		(List.map (fun x -> zloz h x) ls)::pomocnik t
	| [] -> []
	in List.flatten (pomocnik ls);;
	
zloz [ (1, 2); (2, 1); (3, 3); (4, 4); (5,5) ]
	[ (1, 2); (2, 3); (3, 4); (4, 5); (5, 1) ];;
	
	zloz 
	[ (1, 2); (2, 3); (3, 4); (4, 5); (5, 1) ] [ (1, 2); (2, 1); (3, 3); (4, 4); (5,5) ];;
	
zloz  [(1, 1); (2, 3); (3, 4); (4, 5); (5, 2)] [ (1, 2); (2, 1); (3, 3); (4, 4); (5,5) ];;
