<?php

/*
Przemysław Pastuszka
2010

Edycja profilu
*/

require_once(BASEDIR."includes/view/elements.php");
require_once(BASEDIR."includes/view/view.php");
require_once(BASEDIR."includes/core/database.php");
require_once(BASEDIR."includes/pageElements/profileEditor.php");

function SetMain(View $view, Database $database)
{
	$view -> main = array();
	
	//użytkownik może edytować tylko swój profil. Administrator może edytować wszystkie bez ograniczeń
	if(isset($_GET['nick']) && ($_SESSION['nick'] == $_GET['nick'] || $_SESSION['rank'] == 'administrator'))
	{
		if(isset($_POST['name']))
		{
			$result = $database -> ExecuteQuery(sprintf("update uzytkownik_bez_hasla set imie = '%s', nazwisko = '%s', opis = '%s',
				rodzaj_roweru='%s' where nick='%s'", 
				$database -> EscapeString($_POST['surname']), 
				$database -> EscapeString($_POST['name']), 
				$database -> EscapeString($_POST['desc']), 
				$database -> EscapeString($_POST['bicycle']), 
				$database -> EscapeString($_GET['nick'])));
			if(!$result -> ErrorOccured())
				$view -> main[] = new Text("Profil został pomyślnie zaktualizowany");
			
		}
		else
		{
			$result = $database -> ExecuteQuery(sprintf("select * from uzytkownik_bez_hasla where nick = '%s'", 
				$database -> EscapeString($_GET['nick'])));
			if($result -> ErrorOccured() || $result -> RowsAffected() <= 0)
				$view -> main[] = new Text("Użytkownik o podanym nicku nie istnieje");
			else
			{
				$user = $result -> FetchAssoc();
				$data = new ProfileData();
				$data -> name = $user['nazwisko'];
				$data -> surname = $user['imie'];
				$data -> desc = $user['opis'];
				$data -> bicycle = $user['rodzaj_roweru'];
				$view -> main[] = CreateProfileEditForm($data, "Edycja profilu użytkownika ".
					$database -> EscapeString($_GET['nick']), 
					"index.php?action=editprofile&nick=".$database -> EscapeString($_GET['nick']), false);
			}
		}
	}
	else
		$view -> main[] = new Text("Nie masz uprawnień do wykonania tej akcji");
	return $view;

}

?>
