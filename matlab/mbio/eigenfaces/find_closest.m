function val = find_closest(im, B)
    k = 1;
    min_dist = get_dist(im, B(:,1));
    for i = 2:size(B, 2)
        dist = get_dist(im, B(:,i));
        if dist < min_dist
            min_dist = dist;
            k = i;
        end;
    end;
    val = k;