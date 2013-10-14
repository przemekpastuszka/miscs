fun suffixes [] = [[]]
    | suffixes (ls as h :: t) = ls :: suffixes t
