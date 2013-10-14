function val = read_image(path)
    image = imread(strcat('faces/subject', path));
    image = image(:, 80:260);
    val = double(image(:));