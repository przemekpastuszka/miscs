<?php


/*
Przemysław Pastuszka
2010

Tutaj znajduje się kod dodawania wycieczek
*/

require_once(BASEDIR."includes/view/elements.php");
require_once(BASEDIR."includes/view/view.php");
require_once(BASEDIR."includes/core/database.php");
require_once(BASEDIR."includes/pageElements/tripEditor.php");

function SetMain(View $view, Database $database)
{
	$view -> main = array();
	$showForm = true;
	
	//dodawać wycieczkę może tylko użytkownik bądź administrator
	if($_SESSION['rank'] == 'uzytkownik' || $_SESSION['rank'] == 'administrator')
	{
		if(isset($_POST['trail'])) //czy przesłano formularz
		{
			//dodaj wycieczkę do bazy
			$result = $database -> ExecuteQuery(sprintf("insert into wycieczka(nick, id_trasa, data, opis) values ('%s', '%s', '%s', '%s')",
				$_SESSION['nick'],
				$database -> EscapeString($_POST['trail']),
				$database -> EscapeString($_POST['date']),
				$database -> EscapeString($_POST['desc'])));
			if($result -> ErrorOccured())
				$view -> main[] = new Text("Błędnie podana data lub źle wybrana trasa");
			else
			{
				$view -> main[] = new Text("Wycieczka została dodana");
				$showForm = false;
			}
		}
	}
	else
	{
		$showForm = false;
		$view -> main[] = new Text("Nie masz uprawnień do wykonania tej akcji");
	}
	if($showForm)
	{	//wyświetlenie formularza
		$data = new TripData();
		$data -> desc = isset($_POST['desc']) ? $_POST['desc'] : "";
		$data -> date = isset($_POST['date']) ? $_POST['date'] : "";
		$data -> trail = isset($_POST['trail']) ? $_POST['trail'] : "";
		
		$view -> main[] = CreateTripEditForm($data, "Stwórz wycieczkę", BASEURL."index.php?action=addtrip", $database);
	}	
	return $view;
}
