function val = randFromProbs(probs)
%   randFromProbs(probs) - gets random integer from [1, d] with given
%   probabilities, where d = length(probs)
    d = length(probs);
    r = rand(1);
    sum = 0;
    for i = 1:d
        if r < sum + probs(i)
            val = i;
            break
        end
        sum = sum + probs(i);
    end

