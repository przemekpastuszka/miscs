function val = gauss1d(x, m, s)
%   gauss1d(x,m, s) - computes normal distribution for x
%   m - mean
%   s - variance
	stdDev = sqrt(s);
	squaredTwoPi = sqrt(2 * pi);
	left = 1 / (stdDev * squaredTwoPi);
	right = exp((-(x - m)^2) / (2*s));
	val = left * right;