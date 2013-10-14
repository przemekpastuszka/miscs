expression(X) --> simple_expression(S), expression(X, S).
expression(X, A) --> "*", !, simple_expression(Q), {NA = mul(A, Q)}, expression(X, NA).
expression(X, X) --> "". 

simple_expression(a) --> "a", !.
simple_expression(b) --> "b", !.
simple_expression(X) --> "(", expression(X), ")".
