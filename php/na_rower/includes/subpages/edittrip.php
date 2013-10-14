<?php

/*
Przemysław Pastuszka
2010

Edycja wycieczki
Tutaj też nic ciekawego - tyle, że akurat w tym przypadku edycję zintegrowałem z usuwaniem
*/

require_once(BASEDIR."includes/view/elements.php");
require_once(BASEDIR."includes/view/view.php");
require_once(BASEDIR."includes/core/database.php");
require_once(BASEDIR."includes/pageElements/tripEditor.php");

function SetMain(View $view, Database $database)
{
	$view -> main = array();
	$result = $database -> ExecuteQuery(sprintf("select * from wycieczka where id_wycieczka = '%s'",
		$database -> EscapeString($_GET['id'])));
	if($result -> ErrorOccured() || $result -> RowsAffected() <= 0)
		$view -> main[] = new Text("Wycieczka o podanym id nie istnieje");
	else
	{
		$showForm = true;
		$trip = $result -> FetchAssoc();
		if($_SESSION['rank'] == 'administrator' || ($_SESSION['rank'] == 'uzytkownik' && $_SESSION['nick'] == $trip['nick']))
		{
			if(isset($_POST['trail']))
			{
				$result = $database -> ExecuteQuery(sprintf("update wycieczka_modyfikacja set id_trasa = '%s', data = '%s', opis = '%s'
					where id_wycieczka = '%s'",
					$database -> EscapeString($_POST['trail']),
					$database -> EscapeString($_POST['date']),
					$database -> EscapeString($_POST['desc']),
					$trip['id_wycieczka']));
				if($result -> ErrorOccured())
					$view -> main[] = new Text("Błędnie podana data lub źle wybrana trasa");
				else
				{
					$view -> main[] = new Text("Wycieczka została zmodyfikowana");
					$showForm = false;
				}
			}
			
			//o, tu właśnie sprawdzamy żądanie usunięcia
			if(isset($_POST['delete']))
			{
				$result = $database -> ExecuteQuery(sprintf("delete from wycieczka where id_wycieczka = '%s'",
					$trip['id_wycieczka']));
				$view -> main[] = new Text("Wycieczka została usunięta");
				$showForm = false;
			}
			
			if($showForm)
			{
				$data = new TripData();
				$data -> desc = $trip['opis'];
				$data -> date = date("d-m-Y", strtotime($trip['data']));
				$data -> trail = $trip['id_trasa'];
		
				$view -> main[] = CreateTripEditForm($data, "Edytuj wycieczkę", BASEURL."index.php?action=edittrip&id=".$trip['id_wycieczka'], $database);
				//a tu wyświetlamy formularz, który pozwala na wysunięcie takiego żądania
				$view -> main[] = new Form("", BASEURL."index.php?action=edittrip&id=".$trip['id_wycieczka'],
					array(new InputText("delete", "Usuń wycieczkę", "", 30, "hidden")));
			}
		}
		else
			$view -> main[] = new Text("Nie masz uprawnień do wykonania tej akcji");
	}
		
	return $view;
}
