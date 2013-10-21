function iris()
    addpath(genpath('/home/przemek/code/miscs/matlab/neuronal/prac2/minFunc'))
    load fisheriris;
    meas_shifted = bsxfun(@minus, meas, min(meas));
    meas_normalized = bsxfun(@rdivide, meas_shifted, max(meas_shifted)).*2.-1;
    
    A=[ones(150,1), ones(150,1) * 2, ones(150,1)*3];
    [~,~,IC] = unique(species);
    expected_output = bsxfun(@eq, A, IC);
    
    check(@least_squares, @minFunc, 2, meas_normalized', expected_output');
    check(@least_squares, @minFunc, 3, meas_normalized', expected_output');
    check(@least_squares, @minFunc, 5, meas_normalized', expected_output');
    
    check(@cross_entropy, @minFunc, 2, meas_normalized', expected_output');
    check(@cross_entropy, @minFunc, 3, meas_normalized', expected_output');
    check(@cross_entropy, @minFunc, 5, meas_normalized', expected_output');

    
function check(loss, minimalization, n, IN, OUT)
    all_mistakes = 0;
    for i = 1:5
        vec = 0.6 * rand(8 * n + 3, 1) - 0.3;
        options = [];
        options.Display = 'off';
        min_arg = minimalization(loss, vec, options, IN, OUT);
        all_mistakes = all_mistakes + mistake_count(loss, min_arg, IN, OUT); 
    end
    disp(sprintf('Function %s with %d neurons makes %f mistakes on average', func2str(loss), n, all_mistakes / 5))
        

