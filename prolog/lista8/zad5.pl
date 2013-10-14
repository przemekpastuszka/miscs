tree(leaf) --> "*", !.
tree(node(X, Y)) --> "(", tree(X), tree(Y), ")".
