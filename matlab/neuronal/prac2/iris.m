function iris()
    addpath(genpath('/home/przemek/code/matlab/neuronal/prac2/minFunc'))
    load fisheriris;
    meas_shifted = bsxfun(@minus, meas, min(meas));
    meas_normalized = bsxfun(@rdivide, meas_shifted, max(meas_shifted)).*2.-1;
    
    A=[ones(150,1), ones(150,1) * 2, ones(150,1)*3];
    [~,~,IC] = unique(species);
    expected_output = bsxfun(@eq, A, IC);
    
    n = 2;
    vec = 0.6 * rand(8 * n + 3, 1) - 0.3;
    [arg, val] = minFunc(@least_squares, vec, [], meas_normalized', expected_output')

