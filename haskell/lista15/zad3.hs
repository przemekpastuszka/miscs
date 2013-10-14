import IO;

data Request = PutStrLn String | ReadLine
data Response = OK | OKStr String
type Dialog = [Response] -> [Request]

f [] = return []
f (PutStrLn s : r) = do
	putStrLn s
	ls <- f r
	return $ OK : ls
f (ReadLine : r) = do
	s <- getLine
	ls <- f r
	return $ OKStr s : ls



{--dialogToIOMonad :: Dialog -> IO ()
dialogToIOMonad dialog =
	do
		w <- f (dialog w)
		return ()--}
	
