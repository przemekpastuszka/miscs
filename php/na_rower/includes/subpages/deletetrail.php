<?php

/*
Przemysław Pastuszka
2010

Usuwanie trasy
Kod podobny jak w przypadku kategorii
*/

require_once(BASEDIR."includes/view/view.php");
require_once(BASEDIR."includes/core/database.php");

function SetMain(View $view, Database $database)
{
	$view -> main = array();
	
	if(isset($_GET['id']) && $_SESSION['rank'] == 'administrator')
	{
		$resultA = $database -> ExecuteQuery(sprintf("select zdjecie from trasa where id_trasa = '%s'", 
			$database -> EscapeString($_GET['id'])));
		$result = $database -> ExecuteQuery(sprintf("delete from trasa where id_trasa = '%s'", 
			$database -> EscapeString($_GET['id'])));
		if($result -> ErrorOccured())
			$view -> main[] = new Text($result -> GetError());
		else
		{
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
