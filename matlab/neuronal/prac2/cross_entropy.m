function [error, grad, L_2] = cross_entropy(vec, IN, OUT)
    [W_1, W_2, bias_1, bias_2] = decode_theta(vec);
    [L_2, ~, L_1, ~] = two_layer_single(IN, W_1, W_2, bias_1, bias_2, @tanh, @softmax);
    
    error = -sum(log(sum(L_2.*OUT)));
    grad_a_2 = L_2 - OUT;
    grad = compute_gradient(IN, L_1, W_2, grad_a_2);