import IO;

main = do
	putStrLn "Choose file to open: "
	source <- getLine
	content <- readFile source
	putStrLn "Choose destination: "
	dest <- getLine
	writeFile dest content
