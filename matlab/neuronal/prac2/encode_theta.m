function vec = encode_theta(W_1, W_2, bias_1, bias_2)
    vec = [W_1(:); W_2(:); bias_1; bias_2];