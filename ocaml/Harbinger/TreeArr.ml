(********************
Projekt Harbinger
Przemys³aw Pastuszka
Zmodyfikowano: 19.12.2009

Plik zawiera modu³ do obs³ugi tablicy funkcjnej opartej na drzewie wyszukiwañ binarnych zdefiniowanym w module BTree
********************)


type 'b treeArray=int*(int,'b) BTree.bTree;;

(*elementu w drzewie bêdziemy szukaæ wg nastêpuj¹cego schematu:
jeœli klucz =1 to jesteœmy na miejscu
jeœli klucz parzysty to szukaj w lewym poddrzewie
wpp w prawym

jeœli schodzisz w dó³ to podziel klucz przez dwa*)
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
		Not_found -> failwith "Internal error in (function get, module TreeArr) occurred. Sorry";; (*to siê nie powinno zdarzyæ*)
		
		
(*Jeœli wybrano element spoœród istniej¹cych to wykonujemy zwyk³y update
Jeœli nie, to dodajemy element i uzupe³niamy drzewo domyœln¹ wartoœci¹, aby pozosta³o zupe³ne*)
let update (nr,tree) (i,value) def=
	let rec helper t i n= (*uzupe³nia drzewo*)
		if i>n then
			t
		else
			helper (BTree.add  ~cmp:comparer ~modify:modifier t (i,def)) (i+1) n in
	if i<=0 then
		raise IndexOutOfBounds
	else if i>nr then
		(i,BTree.add ~cmp:comparer ~modify:modifier (helper tree (nr+1) (i-1)) (i,value)) (*trzeba dodaæ element i uzupe³niæ drzewo*)
	else
		try (nr,BTree.update ~cmp:comparer ~modify:modifier tree (i,value)) with (*a tu wystarczy update*)
			Not_found -> failwith "Internal error in (function update, module TreeArr) occurred. Sorry";; (*to siê nie powinno zdarzyæ*)
		
	