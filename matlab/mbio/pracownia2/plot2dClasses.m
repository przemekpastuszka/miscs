function val = plot2dClasses(X, y, ms)
% plot2dClasses(X, y, ms) - plots the 2d samples from different classes
% X - Nx2 array of samples
% y - vector indicating samples membership
% ms - Nx2 array of mean vectors for each class
    N = length(y);
    c = max(y);

    %plot values
    hold on
    for i = 1:N
        color = getColor(y(i));
        plot(X(i, 1), X(i, 2), 'MarkerSize', 10, 'Marker', '+', 'MarkerEdgeColor', color, 'LineStyle', 'none');
    end
    hold off

    % plot the mean
    hold on;
    for i = 1:c
        color = getColor(i);
        plot(ms(i, 1), ms(i, 2), 'MarkerSize', 20, 'Marker', 'o', 'MarkerEdgeColor', color, 'LineStyle', 'none');
    end
    hold off;
    val = 1;
  
function val = getColor(n)
% getColor(n) - returns [r g b] vector for given natural number
% unique colors are defined for n in [1, ..., 8] as function works as
% follows:
%   - convert (n - 1) to binary format
%   - three last digits are mapped to r g b
%   e.g. for n = 3 we have:
%   n - 1 = 2 = 010(b) = [r=0, g=1, b=0] = green
    n = n - 1;
    color = [];
    for i = 0:2
        color = [mod(floor(n / 2^i), 2) color];
    end
    val = color;