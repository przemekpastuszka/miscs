<?php

/*
Przemysław Pastuszka
2010
*/

//funkcja do ładowania obrazków
function LoadImage($image, $size, $destination)
{
	
	if(isset($_FILES[$image]['tmp_name']) &&  is_uploaded_file($_FILES[$image]['tmp_name']))
	{
		if($_FILES[$image]['size'] > $size)
			throw new Exception('Przesłany plik jest zbyt duży!');
		
		$tmp = explode('.', $_FILES[$image]['name']);
		$n = count($tmp) - 1;
		$extension = $tmp[$n];
	
		if(!in_array($extension, array("jpg", "png", "gif")))
			throw new Exception('Nieprawidłowy format zdjęcia. Obsługiwane formaty to: .png, .jpg, .gif.');
	
		//tak jak wspominałem w modelu - hashujemy czas, by uniknąć kolizji w nazewnictwie
		$newName = sha1(microtime()).'.'.$extension;
		if(!move_uploaded_file($_FILES[$image]['tmp_name'], $destination.$newName))
			throw new Exception('Wystąpił błąd systemu w trakcie próby przetworzenia zdjęcia. Przepraszamy');
		
		return $newName;
	}
	return "";
}

?>
