function val = make2Dplot(X, y, ms)
    N = length(y);
    c = max(y);

    % group objects from the same classes
    objectsByClass{c} = [];
    for i = 1:N
        objectsByClass{y(i)} = [objectsByClass{y(i)} X(i)];
    end

    % draw plot
    hold on;
    for i = 1:c
        color = getColor(i);
        % plot the markers
        plot(objectsByClass{i}, 'MarkerSize', 10, 'Marker', '+', 'MarkerEdgeColor', color, 'LineStyle', 'none');
        hold all;
        % plot mean vector
        d = length(objectsByClass{i});
        plot([1:d], repmat(ms(i), 1, d), 'Color', color);
        hold all;
    end
    hold off;
    val = 1;

function val = getColor(n)
    n = n - 1;
    color = [];
    for i = 0:2
        color = [mod(floor(n / 2^i), 2) color];
    end
    val = color;
