function val = randNormalDimensional(N, m, S)
%   randNormalDimensional(m, s) - generates Nxd matrice of random numbers with normal
%   distribution, where d = length(m)
%   m - mean vector
%   S - covariance matrix
    R = chol(S);
    v = [];
    d = length(m);
    for i = 1:N
        x = m' + R * randn(d, 1);
        v = [v x];
    end
    val = v';
