function res = main(file)
    read_file = csvread(file);
    X = read_file(:,1);
    Y = read_file(:,2);
    A = array_out_of(X);
    res = BGD(@(z) least_squares(z, A, Y), [0, 0, 0]', 10^-10)
    analytical = (inv(A'*A))*A'*Y

    subplot(2,1,2);
    plot(X, Y, 'b.');
    hold on;
    xs = linspace(min(X), max(X), size(X, 1));
    plot(xs, array_out_of(xs') * res, 'r');
    hold on;
    plot(xs, array_out_of(xs') * analytical, 'g');
    hold off;

function val = array_out_of(X)
 val = [ones(size(X)), X, X.^2];