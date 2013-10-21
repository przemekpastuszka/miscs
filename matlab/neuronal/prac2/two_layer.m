function output = two_layer(IN, W_1, W_2, bias_1, bias_2, f_1, f_2)
  L_1 = compute_layer(IN, W_1, bias_1, f_1);
  output = compute_layer(L_1, W_2, bias_2, f_2);
