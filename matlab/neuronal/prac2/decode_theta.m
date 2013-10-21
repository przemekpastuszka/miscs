function [W_1, W_2, bias_1, bias_2] = decode_theta(vec)
    n = (size(vec, 1) - 3) / 8;
    W_1 = reshape(vec(1:n*4), n, 4);
    W_2 = reshape(vec(n*4+1:7*n), 3, n);
    bias_1 = vec(7*n+1:8*n);
    bias_2 = vec(8*n+1:8*n+3);