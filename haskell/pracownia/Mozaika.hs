{--
Pracownia z programowania nr 3
Przemysław Pastuszka
2010

Rozwiązanie jest bardzo proste - generuj wszystkie (niemal) możliwe kombinacje liczb i sprawdź czy są rozwiązaniami
Oczywiście dodatkowo to pracy zaprzęgnięte jest odsiewanie liczb, które na pewno nie znajdą się w rozwiązaniu
Szczegóły w komentarzach w kodzie

Program, po skompilowaniu, należy uruchomić z linii poleceń, jako atrybut podając mu nazwę pliku z danymi
--}

import Control.Monad;
import System.Environment;
import List;

{------------------------------------------------------------------------}
--WEJŚCIE
{------------------------------------------------------------------------}

readInt :: String -> Integer
readInt = read

readListInt :: ReadS [[Integer]]
readListInt = readList --bardzo sprytna i przydatna funkcja z Prelude

processStringAndSolve s =
	let 
		n = readInt $ takeWhile (\x -> x /= '.') s --pobierz wymiar planszy
		ls = fst $ head $ readListInt $ dropWhile (\x -> x/= '[') s --wczytaj planszę
	in solve ls n --rozwiąż
	
main = do
	[fileName] <- getArgs --pobranie argumentu z wiersza poleceń
	input <- readFile fileName
	writeOutput $ processStringAndSolve input


{------------------------------------------------------------------------}
--WYJŚCIE

writeOutput [] = return () --tu się nic ciekawego nie dzieje
writeOutput (h : t) = do
	writeElement h
	putStrLn ""
	writeOutput t

writeElement (a, b) = do
	putStr "("
	putStr $ show a
	putStrLn ","
	putStr "["
	writeList b
	putStrLn "])"

writeList [] = undefined
writeList [h] = do
	putStr $ show $ snd h
	return ()
writeList (h : t) = do
	putStr $ show $ snd h
	putStrLn ","
	writeList t
	
	
{------------------------------------------------------------------------}
--PREPROCESSING

--funkcje pomocnicze
hd (h : _) = Just h
hd [] = Nothing

toSingleton (Just x) = [x]
toSingleton Nothing = []

--dodaje informację o pozycji (y, x) do każdej wartości na planszy	
markWithPositions ls = do
	(nr, row) <- zip [1..length ls] ls
	return $ zip row [(nr, x) | x <- [1..length row]]

--poniżej kod służący do pobrania wartości na danej pozycji
getAtPosition ls y x
	| y <= 0 || x <= 0 = Nothing
	| otherwise = do --do obsługi błędów wykorzystywana jest monada Maybe
		rest <- return $ drop (y - 1) ls
		row <- hd rest --znajdź wiersz
		rest2 <- return $ drop (x - 1) row
		hd rest2

--kod znajdujący wszystkich sąsiadów pola o podanej pozycji
getNeighbours ls y x = do
	offX <- [-1, 0, 1]
	offY <- [-1, 0, 1]
	guard $ offX /= 0 || offY /= 0 --relacja sąsiedztwa nie jest zwrotna
	toSingleton $ getAtPosition ls (y + offY) (x + offX)

--kod do generowania wszystkich możliwych iloczynów spośród liczb umieszczonych na planszy
getAllProducts ls = do
	row <- ls
	(value, (y, x)) <- row
	guard $ value /= 0 --iloczyn musi być dwucyfrowy, więc odsiewamy przypadki, gdy na początku stoi zero
	(value2, (y2, x2)) <- getNeighbours ls y x
	return $ (value * 10 + value2, ((y, x), (y2, x2)))
	
--generowanie liczb pierwszych
primes = 2 : [ n | n <- [3..], all (\ p -> mod n p /= 0)
                      (takeWhile (\p -> p*p <= n) primes) ]
                      
--jeśli n > 2 to na pewno ze wszyskich iloczynów możemy odsiać iloczyny pierwsze (dowód jest bardzo prosty)
sievePrimes n ls
	| n <= 2 = ls
	| otherwise =
		filter (\(x,_) -> notElem x (takeWhile (<100) primes)) ls
		
--przygotowanie wejściowych danych do procedury rozwiązywania zagadki. Przygotowanie składa się z kilku etapów:
--	przypisz polom informacje o pozycji
--	znajdź wszystkie możliwe iloczyny
--	wysiej z nich liczby pierwsze (jeśli n > 1)
--	posortuj i pogrupuj
prepareInput ls n =
	groupBy (\ (a,_) (b,_) -> a == b) $ sort $ sievePrimes n $ getAllProducts $ markWithPositions ls
	
--zwraca wszystkie dzielniki danej liczby
getDivisors n =
	n : filter (\p -> mod n p == 0) [1..(div n 2)]

--każde z możliwych rozwiazań musi być dzielnikiem jakiegoś iloczynu	
getPossibleSolutions ls =
	sort $ nub $ concatMap getDivisors products
	where products = map (fst.head) ls

--znów łatwo udowodnić, że w przypadku n > 2 w wyniku nie może pojawić się 1 ani żadna liczba >= 25
sieveImpossible n ls
	| n <= 2 = ls
	| otherwise = filter (<25) $ delete 1 ls


{------------------------------------------------------------------------}
--SILNIK ROZWIĄZANIA

--byłem zmuszony napisać swoją funkcję tails, gdyż standardowa zwraca również [], co jest niepożądane
myTails [] = undefined
myTails [h] = [[h]]
myTails ls@(h : t) = ls : myTails t

solve ls n =
	do
		rest@(h : t) <- myTails possibleSolutions --generujemy pierwszy element, który znajdzie się na liście rozwiązania
		findSolutions [h] [] (n - 1) preparedInput rest --znajdujemy resztę
	where
		preparedInput = prepareInput ls n
		possibleSolutions = reverse $ sieveImpossible n $ getPossibleSolutions preparedInput


--funkcja, która wyszukuje kolejne liczby, jakie znajdą się na liście rozwiązania
findSolutions numbers products 0 _ _ = return (numbers, products) --nie potrzeba więcej elementów - zwróc rozwiązanie
findSolutions numbers products n all [] = [] --brakło elementów do sprawdzenia, tj. wcześniejsze wybory musiały być nietrafione
findSolutions numbers products n all toCheck = do
	rest@(h : t) <- myTails toCheck --wybierz kandydata ('h') na następną liczbę w rozwiązaniu
	newProducts <- generateNewProducts numbers products [] h all --wygeneruj wszystkie poprawne iloczyny z poprzednimi liczbami
	findSolutions (h : numbers) (newProducts ++ products) (n - 1) all rest --znajdź resztę liczb

--proste sprawdzenie, czy dwa iloczyny na planszy nie nachodzą na siebie
notCollide (_, (a, b)) ls = all (\(_, (c, d)) -> a /= c && b /= d && a /= d && b /= c) ls

--generuj wszystkie możliwe iloczyny nowej liczby (newNumber) z poprzednio wybranymi liczbami (pierwszy argument)
--oczywiście iloczyny muszą być poprawne, tj. znajdować się na planszy i nie kolidować z poprzednio stworzonymi iloczynami	
generateNewProducts [] oldProducts newProducts newNumber all = return $ reverse newProducts
generateNewProducts (h : t) oldProducts newProducts newNumber all = do
	newPossibleProducts <- all --przejrzyj iloczyny na planszy
	guard $ (fst.head) newPossibleProducts == newNumber * h
	currentProduct <- newPossibleProducts --wybierz jeden z iloczynów o tej samej wartości
	guard $ notCollide currentProduct oldProducts --sprawdź czy nie ma kolizji ze starymi
	guard $ notCollide currentProduct newProducts --i nowymi iloczynami
	generateNewProducts t oldProducts (currentProduct : newProducts) newNumber all --znajdź resztę iloczynów
