function val = read_faces(sets)
    A = [];
    for j = 1:size(sets, 2)
        for i = 1:15
            image = read_image(strcat(num2str(i,'%02d'), sets{j}));
            A = [ A image ];
        end;
    end;
    val = A;