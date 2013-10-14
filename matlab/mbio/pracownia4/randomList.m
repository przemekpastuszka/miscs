function val = randomList(k, d)
    ls{k} = 0;
    for i = 1:k
        ls{i} = randn([1 d]);
    end
    val = ls;