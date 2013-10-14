function val = numerical_gradient(f, x)
    epsilon = 10^-5;
    x_size = size(x, 1);
    left = apply_to_each_column(f, repmat(x, 1, x_size) + epsilon * eye(x_size));
    right = apply_to_each_column(f, repmat(x, 1, x_size) - epsilon * eye(x_size));
    val = (left - right) / (2 * epsilon);
    
 function val = apply_to_each_column(f, M)
     numRows = size(M, 1);
     val = zeros(numRows, 1);
     for i=1:numRows
         row = M(:,i);
         val(i) = f(row);
     end