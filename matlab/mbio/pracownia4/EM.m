function val = EM(X, k)
    km = my_kmeans(X, k);
    xs = vec2list(X');
    
    N = length(xs);
    d = length(xs{1});
    ms = vec2list(km{2}');
    ss = vec2list(ones(1, k)');
    
    pis{k} = 0;
    for i = 1:k
        pis{i} = length(km{1}{i}) / N;
    end
    
    for t = 1:80 % 30 iterations
        A = gammaMatrix(xs, ms, ss, pis);
        gammaSumPerClass = sum(A);
        for i = 1:k
            result = A(1, i) * xs{1};
            for j = 2:N
                result = result + A(j, i) * xs{j};
            end
            ms{i} = result / gammaSumPerClass(i);
        end
        
        for i = 1:k
            result = A(1, i) * my_dot(xs{1} - ms{i});
            for j = 2:N
                result = result + A(j, i) * my_dot(xs{j} - ms{i});
            end
            ss{i} = result / gammaSumPerClass(i);
        end
        
        for i = 1:k
            pis{i} = gammaSumPerClass(i) / N;
        end
    end
    
    val = {ms, ss, pis};
    
    function val = my_dot(x)
        val = x * x';