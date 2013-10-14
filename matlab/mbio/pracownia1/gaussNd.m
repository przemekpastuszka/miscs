function val = gaussNd(x, m, covar)
%   gaussNd([x],[m], [s]) - computes n-dimensional normal distribution for [x]
%   [m] - mean vector
%   [s] - covariance array
    n = length(x);
    left = 1 / (((2 * pi) ^ (n/2)) * sqrt(det(covar)));
    
    v = (x - m);
    expLeft = v * inv(covar);
    right = exp(-(1/2) * expLeft * v');
	val = left * right;