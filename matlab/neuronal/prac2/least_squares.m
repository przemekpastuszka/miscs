function [error, grad] = least_squares(vec, IN, OUT)
    error = compute_error(IN, OUT, vec);
    grad = numerical_gradient(@(x) compute_error(IN, OUT, x), vec);
    
function error = compute_error(IN, OUT, vec)
    n = size(IN, 1);
    error = 0;
    for i = 1:n
        result = two_layer_single(IN(i,:)', vec, @tanh, @tanh);
        error = error + norm((result - OUT(i,:)').^2, 1);
    end