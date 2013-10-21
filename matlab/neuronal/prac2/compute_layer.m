function output = compute_layer(IN, W, bias, f)
  A = W * IN + bias;
  output = f(A);