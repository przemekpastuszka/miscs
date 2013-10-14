(*********************************
Przechowuje funkcje i struktury w ¶rodowisku
*********************************)

(*lista asocjacyjna funkcji w œrodowisku Harbinger*)
let functions=ref [];;
let structs=ref [];;

(*let getFunctions ()=functions;;*)
let get key ls=List.assoc key (!ls);;
let remove key ls=
	ls:=List.remove_assoc key (!ls);;
let add (key,value) ls=
	ls:=(key,value)::(!ls);;
let update (key,value) ls=
	remove key ls;add (key,value) ls;;
let clear x=
	x:=[];;
