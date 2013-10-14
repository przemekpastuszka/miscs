function val = randNormal(N, m, s)
%   randNormal(m, s) - generates Nx1 vector of random numbers with normal
%   distribution
%   m - mean
%   s - std dev
    vec = randn(N, 1);
    val = (vec.*s) + m;
