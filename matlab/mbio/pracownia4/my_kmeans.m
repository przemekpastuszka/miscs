function val = my_kmeans(X, k)
    clusters = X(:,1:k);
    
    for i = 1:30 % 30 iterations
        objectsByClass = assignPoints(clusters, X, k);
        clusters = updateClusters(clusters, objectsByClass, k);
    end
    
    val = {objectsByClass, clusters};


function val = updateClusters(clusters, pointsByClass, k)
    newClusters = [];
    for i = 1:k
        q = size(pointsByClass{i});
        if q(2) == 1
            newClusters = [newClusters pointsByClass{i}];
        else
            newClusters = [newClusters mean(pointsByClass{i}')'];
        end
    end
    val = newClusters;
    
function val = assignPoints(clusters, points, k)
    N = length(points);
    objectsByClass{k} = [];
    for i = 1:N
        point = points(:,i:i);
        min = my_dist(point, clusters(:,1:1));
        candidate = 1;
        for j = 2:k
            dist = my_dist(point, clusters(:,j:j));
            if dist < min
                min = dist;
                candidate = j;
            end
        end
        objectsByClass{candidate} = [objectsByClass{candidate} point];
    end
    val = objectsByClass;
    
function val = my_dist(x, y) 
        v = x - y;
        val = sum(v.*v);