function [error, grad] = cross_entropy(vec, IN, OUT)
    error = compute_error(IN, OUT, vec);
    grad = numerical_gradient(@(x) compute_error(IN, OUT, x), vec);
    
function error = compute_error(IN, OUT, vec)
    n = size(IN, 1);
    error = 0;
    for i = 1:n
        result = two_layer_single(IN(i,:)', vec, @tanh, @softmax);
        error = error - log(norm(result.*OUT(i,:)', 1));
    end