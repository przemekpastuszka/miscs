function grad = compute_gradient(IN, L_1, W_2, grad_a_2) 
    grad_w_2 = grad_a_2 * L_1';
    grad_bias_2 = sum(grad_a_2')';
    grad_l_1 = W_2' * grad_a_2;
    grad_a_1 = grad_l_1 - grad_l_1.*L_1.^2;
    grad_w_1 = grad_a_1 * IN';
    grad_bias_1 = sum(grad_a_1')';
    grad = encode_theta(grad_w_1, grad_w_2, grad_bias_1, grad_bias_2);