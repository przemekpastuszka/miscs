training_set = {'.normal', '.happy', '.surprised'};
test_set = {'.wink', '.sleepy', '.sad'};
ks = [1 4 7 10 15]; % how many principal components should be used

A = read_faces(training_set);
V = compute_eigenfaces(A, max(ks));

for i = 1:size(ks, 2)
    disp(strcat(['Matches for ', num2str(ks(i)), ' principal components:']));
    classify(A, V(:,1:ks(i)), training_set, test_set);
end;