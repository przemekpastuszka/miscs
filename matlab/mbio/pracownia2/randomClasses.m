function val = randomClasses(N, ms, Ss, probs)
%   randomClasses(N, ms, Ss, probs) - generates Nxd matrice of random numbers with normal
% distribution N(ms{i}, Ss{i}). i is chosen randomly using probabilities given in
% probs
    X = [];
    y = [];
    for i = 1:N
        k = randFromProbs(probs);
        x = randNormalDimensional(1, ms{k}, Ss{k});
        X = [X;x];
        y = [y;k];
    end
    val = {X y};
    
