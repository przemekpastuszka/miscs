f n p k =
	if mod p k == 1 then n
	else f (n + 1) (2*p) k
g = f 1 2 
