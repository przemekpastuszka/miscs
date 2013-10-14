function [j_val, gradient] = least_squares(theta, X, Y)
  diffs = X * theta - Y;
  j_val = 0.5 * diffs' * diffs;
  gradient = X'*X*theta - X'*Y;