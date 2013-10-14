module Test where
import List
import Char
integerToString 0 = "0"
integerToString i =
	reverse (List.unfoldr (\x -> if x == 0 then Nothing else Just (Char.intToDigit (mod x 10), div x 10)) i)
