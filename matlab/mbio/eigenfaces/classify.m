function val = classify(A, V, training_set, test_set)
    test_set_size = size(test_set, 2);
    B = V' * A;
    
    disp('Wrong matches: ');
    matches = 0;
    for i = 1:test_set_size
        for j = 1:15
            im_id = num2str(j,'%02d');
            im_name = strcat(im_id, test_set{i});
            im = read_image(im_name);
            id = find_closest(V' * im, B);

            name = get_name_from_id(id, training_set);
            cmp = strcmp(strtok(name, '.'), im_id); % is person the same
            matches = matches + cmp;
            if cmp == 0  % print only wrong matches
                disp(strcat([im_name, ' -> ', name]));
            end;
        end;
    end;
    
    disp(strcat(['There were ', int2str(matches), ' good matches and ', ...
        int2str(15 * test_set_size - matches), ' wrong.']));
    
    val = matches;