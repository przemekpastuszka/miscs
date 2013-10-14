function val = get_name_from_id(x, sets)
    x = x - 1;
    subjectId = mod(x, 15) + 1;
    setId = floor(x / 15) + 1;
    val = strcat(num2str(subjectId,'%02d'), sets{setId});