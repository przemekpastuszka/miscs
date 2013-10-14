function val = gammaMatrix(xs, ms, ss, pis)
    N = length(xs);
    k = length(ms);
    d = length(ms{1});
    
    A = zeros(N, k);
    
    for i = 1:N
        for j = 1:k
            A(i, j) = easyGauss(xs{i}, ms{j}, ss{j}) * pis{j};
        end
    end
    
    B = sum(A');
    
    for i = 1:N
        for j = 1:k
            A(i, j) = A(i, j) / B(i);
        end
    end
    
    val = A;