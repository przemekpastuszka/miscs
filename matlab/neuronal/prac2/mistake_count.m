function mistakes = mistake_count(f, vec, IN, OUT)
    [~, ~, result] = f(vec, IN, OUT);
    are_relevant_positions_big = bsxfun(@ge, sum(result.*OUT), 0.7);
    are_irrelevant_positions_small = bsxfun(@le, sum(result.*(1.-OUT)), 0.3);
    mistakes = sum(bsxfun(@ne, are_relevant_positions_big + are_irrelevant_positions_small, 2));