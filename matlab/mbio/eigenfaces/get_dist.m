function val = get_dist(im, im2) % use usual Euclidean metric
    Z = im - im2;
    val = Z' * Z;