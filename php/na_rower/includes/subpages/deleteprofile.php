<?php

/*
Przemysław Pastuszka
2010

Usuwanie profilu
Nic się ciekawego nie dzieje
*/

require_once(BASEDIR."includes/view/view.php");
require_once(BASEDIR."includes/core/database.php");

function SetMain(View $view, Database $database)
{
	$view -> main = array();
	
	if(isset($_GET['nick']) && $_SESSION['rank'] == 'administrator' && $_GET['nick'] != 'admin')
	{
		$result = $database -> ExecuteQuery(sprintf("delete from uzytkownik where nick = '%s'", 
			$database -> EscapeString($_GET['nick'])));
		if($result -> ErrorOccured())
			$view -> main[] = new Text("Użytkownik o podanym nicku nie istnieje");
		else
			$view -> main[] = new Text("Operacja wykonana poprawnie");
	}
	else
		$view -> main[] = new Text("Nie masz uprawnień do wykonania tej akcji");
	return $view;
}

?>
