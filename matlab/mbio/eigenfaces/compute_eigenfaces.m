function val = compute_eigenfaces(A, k)
    mu =  mean( A, 2 );
    A = A - repmat( mu, 1,  size(A,2) );
    [ UU , ~, ~ ] = svds( A, k );
    val = UU;