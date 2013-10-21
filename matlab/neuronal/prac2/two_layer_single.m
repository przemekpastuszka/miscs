function [L_2, A_2, L_1, A_1] = two_layer_single(IN, W_1, W_2, bias_1, bias_2, f_1, f_2)
    [L_1, A_1] = compute_layer(IN, W_1, bias_1, f_1);
    [L_2, A_2] = compute_layer(L_1, W_2, bias_2, f_2); 

function [L, A] = compute_layer(IN, W, bias, f)
    A = bsxfun(@plus, W * IN, bias);
    L = f(A);