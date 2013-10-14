len([], 0).

len([_|T], Y) :- 
	var(Y), 
	len(T, Q), 
	Y is Q + 1.
	
len([_|T], Y) :- 
	not(var(Y)), 
	Y > 0, 
	Q is Y - 1, 
	len(T, Q).
