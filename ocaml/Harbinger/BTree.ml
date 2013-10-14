(********************
Projekt Harbinger
Przemys�aw Pastuszka
Zmodyfikowano: 18.12.2009

Plik zawiera binarne drzewo poszukiwa� przechowuj�ce pary (klucz,warto��)
Jako �e klucze mog� by� dowolnego typu nale�y poda� r�wnie� funkcj� por�wnuj�c� (-1, gdy pierwszy jest mniejszy, 0 gdy r�wne i 1 gdy drugi mniejszy) - standardowo zwyk�e por�wnanie
UWAGA: Mozna r�wnie� zdefiniowa� funkcj� zmieniaj�c� warto�� poszukiwanego klucza przy ka�dym zej�ciu na ni�szy poziom (standardowo identyczno��)
Sam kod nie wymaga komentarzy
********************)

let stdCmp a b=
	if a<b then
		-1
	else if a>b then
			1
		else
			0;;

type ('a,'b) bTree = Leaf | Node of ('a,'b) bTree * ('a*'b) * ('a,'b) bTree;;

let empty=Leaf;;

exception BadComparer;;

let get ?(cmp=stdCmp) ?(modify=(fun x->x)) tree key =
	let rec execute subTree nKey=
		match subTree with
		Leaf -> raise Not_found
		| Node (left,(k,v),right)->
		(
			match cmp nKey k with
			-1 -> execute left (modify nKey)
			|0 -> v
			|1 -> execute right (modify nKey)
			|_->raise BadComparer
		) in
		execute tree key;;
		
let add ?(cmp=stdCmp) ?(modify=(fun x->x)) tree (key,value) =
	let rec execute subTree nKey=
		match subTree with
		Leaf -> Node (Leaf,(nKey,value),Leaf)
		| Node (left,(k,v),right)->
		(
			match cmp nKey k with
			-1 -> Node (execute left (modify nKey),(k,v),right)
			|0 -> Node (left,(k,v),right)
			|1 -> Node (left,(k,v),execute right (modify nKey))
			|_->raise BadComparer
		) in
		execute tree key;;
		
let update ?(cmp=stdCmp) ?(modify=(fun x->x)) tree (key,value) =
	let rec execute subTree nKey=
		match subTree with
		Leaf -> raise Not_found
		| Node (left,(k,v),right)->
		(
			match cmp nKey k with
			-1 -> Node (execute left (modify nKey),(k,v),right)
			|0 -> Node (left,(k,value),right)
			|1 -> Node (left,(k,v),execute right (modify nKey))
			|_->raise BadComparer
		) in
		execute tree key;;
		
(*let remove tree cmp modify (key,value)=
	let rec execute subTree nKey=
		match subTree with
		Leaf -> Leaf
		| Node (left,(k,v),right)->
		(
			match cmp nKey k with
			-1 -> Node (execute left (modify nKey),(k,v),right)
			|0 -> 
				(
					match (left,right) with
					(Leaf,Leaf) -> Leaf
					| (Leaf, x) -> x
					| (x, Leaf) -> x
					| 
				)
			|1 -> Node (left,(k,v),execute right (modify nKey))
			|_->raise BadComparer
		) in
		execute tree key;;*)