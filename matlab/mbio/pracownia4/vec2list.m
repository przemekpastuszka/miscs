function val = vec2list(v)
    k = length(v);
    ls{k} = 0;
    for i = 1:k
        ls{i} = v(i,:);
    end
    val = ls;