function [error, grad] = least_squares(vec, IN, OUT)
    m = size(OUT, 2);
    [W_1, W_2, bias_1, bias_2] = decode_theta(vec);
    [L_2, A_2, L_1, ~] = two_layer_single(IN, W_1, W_2, bias_1, bias_2, @tanh, @tanh);
    
    error = sum(sum((L_2 - OUT).^2)) / (2*m);
    grad_a_2 = (A_2 - OUT) ./ m;
    grad = compute_gradient(IN, L_1, W_2, grad_a_2);
    