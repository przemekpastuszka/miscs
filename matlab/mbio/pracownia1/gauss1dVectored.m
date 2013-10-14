function val = gauss1dVectored(x, m, s)
%   gauss1dVectored([x],[m], [s]) - computes normal distribution for [x]
%   [m] - mean
%   [s] - variance
	stdDev = sqrt(s);
	squaredTwoPi = sqrt(2 * pi);
    leftDenominator = stdDev.*squaredTwoPi;
	left = leftDenominator./(leftDenominator.^2);
	right = exp((-(x - m).^2)./(2*s));
	val = left.*right;