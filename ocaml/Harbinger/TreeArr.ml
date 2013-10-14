(********************
Projekt Harbinger
Przemys�aw Pastuszka
Zmodyfikowano: 19.12.2009

Plik zawiera modu� do obs�ugi tablicy funkcjnej opartej na drzewie wyszukiwa� binarnych zdefiniowanym w module BTree
********************)


type 'b treeArray=int*(int,'b) BTree.bTree;;

(*elementu w drzewie b�dziemy szuka� wg nast�puj�cego schematu:
je�li klucz =1 to jeste�my na miejscu
je�li klucz parzysty to szukaj w lewym poddrzewie
wpp w prawym

je�li schodzisz w d� to podziel klucz przez dwa*)
let comparer a b= 
	if a=1 then
		0
	else if a mod 2 =0 then
			-1
	else
			1;;
			
let modifier k=k/2;;

exception IndexOutOfBounds;;			

let empty ()=(0,BTree.Leaf);;

let get (nr,tree) i=
	if i<=0||i>nr then
		raise IndexOutOfBounds
	else
		try BTree.get ~cmp:comparer ~modify:modifier tree i with
		Not_found -> failwith "Internal error in (function get, module TreeArr) occurred. Sorry";; (*to si� nie powinno zdarzy�*)
		
		
(*Je�li wybrano element spo�r�d istniej�cych to wykonujemy zwyk�y update
Je�li nie, to dodajemy element i uzupe�niamy drzewo domy�ln� warto�ci�, aby pozosta�o zupe�ne*)
let update (nr,tree) (i,value) def=
	let rec helper t i n= (*uzupe�nia drzewo*)
		if i>n then
			t
		else
			helper (BTree.add  ~cmp:comparer ~modify:modifier t (i,def)) (i+1) n in
	if i<=0 then
		raise IndexOutOfBounds
	else if i>nr then
		(i,BTree.add ~cmp:comparer ~modify:modifier (helper tree (nr+1) (i-1)) (i,value)) (*trzeba doda� element i uzupe�ni� drzewo*)
	else
		try (nr,BTree.update ~cmp:comparer ~modify:modifier tree (i,value)) with (*a tu wystarczy update*)
			Not_found -> failwith "Internal error in (function update, module TreeArr) occurred. Sorry";; (*to si� nie powinno zdarzy�*)
		
	