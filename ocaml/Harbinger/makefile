make: compile makelib

compile:
	ocamlc -c BTree.mli
	ocamlc -c BTree.ml
	ocamlc -c TreeArr.mli
	ocamlc -c TreeArr.ml
	ocamlc -c HTypes.ml
	ocamlyacc parser.mly
	ocamlc -c parser.mli
	ocamlc -c parser.ml
	ocamllex lexer.mll
	ocamlc -c lexer.ml
	ocamlc -c HInit.mli
	ocamlc -c HData.mli
	ocamlc -c HData.ml
	ocamlc -c HVStack.mli
	ocamlc -c HVStack.ml
	ocamlc -c HInit.ml
	ocamlc -c HCheck.mli
	ocamlc -c HCheck.ml
	ocamlc -c HExecute.mli
	ocamlc -c HExecute.ml
	ocamlc -c Harbinger.ml
	
makelib:
	ocamlc -a -o Harbinger.cma BTree.cmo TreeArr.cmo HTypes.cmo parser.cmo lexer.cmo HData.cmo HInit.cmo HVStack.cmo HCheck.cmo HExecute.cmo Harbinger.cmo
	
clean:
	rm *.cmo *.cmi *.cma lexer.ml parser.ml 
