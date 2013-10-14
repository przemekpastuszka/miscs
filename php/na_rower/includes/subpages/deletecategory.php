<?php

/*
Przemysław Pastuszka
2010

Kod do usuwania kategorii
*/

require_once(BASEDIR."includes/view/view.php");
require_once(BASEDIR."includes/core/database.php");

function SetMain(View $view, Database $database)
{
	$view -> main = array();
	
	if(isset($_GET['name']) && $_SESSION['rank'] == 'administrator')
	{
		//pobieramy nazwę zdjęcia, żeby usunąć je fizycznie z dysku
		$resultA = $database -> ExecuteQuery(sprintf("select zdjecie from kategoria where nazwa_kat = '%s'", 
			$database -> EscapeString($_GET['name'])));
		$result = $database -> ExecuteQuery(sprintf("delete from kategoria where nazwa_kat = '%s'", 
			$database -> EscapeString($_GET['name'])));
		if($result -> ErrorOccured())
			$view -> main[] = new Text($result -> GetError());
		else
		{	//usuwamy zdjęcie z katalogu images
			$cat = $resultA -> FetchAssoc();
			if($cat['zdjecie'] != "")
				unlink(BASEDIR."images/".$cat['zdjecie']);
			$view -> main[] = new Text("Operacja wykonana poprawnie");
		}
	}
	else
		$view -> main[] = new Text("Nie masz uprawnień do wykonania tej akcji");
	return $view;
}

?>
