function val = gaussW3(x)
    val =0.5 * gaussNd(x, [0.5 0.5], eye(2)) + 0.5 * gaussNd(x, [-0.5 0.5], eye(2))
end