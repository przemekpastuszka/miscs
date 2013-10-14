function val = easyGauss(x, m, s)
%   gaussNd([x],[m], [s]) - computes n-dimensional normal distribution for [x]
%   [m] - mean vector
%   [s] - covariance array
    n = length(x);
    left = 1 / (((2 * pi) ^ (n/2)) * sqrt(s));
    
    v = (x - m);
    right = exp(-(1/(2 * s)) * v * v');
	val = left * right;