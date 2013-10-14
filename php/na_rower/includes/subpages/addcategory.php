<?php


/*
Przemysław Pastuszka
2010

W tym pliku znajduje się kod dodawania kategorii
*/

require_once(BASEDIR."includes/view/elements.php");
require_once(BASEDIR."includes/view/view.php");
require_once(BASEDIR."includes/core/database.php");
require_once(BASEDIR."includes/pageElements/categoryEditor.php");
require_once(BASEDIR."includes/utilities/imageLoader.php");

function SetMain(View $view, Database $database)
{
	$view -> main = array();
	$showForm = true; //czy pokazujemy formularz
	//dodawać kategorie może tylko użytkownik lub administrator
	if($_SESSION['rank'] == 'uzytkownik' || $_SESSION['rank'] == 'administrator')
	{
		if(isset($_POST['name']) && $_POST['name'] != "") //jeśli formularz przesłano
		{
			try
			{ 	//ładujemy obrazek
				$image = LoadImage('image', 204800, BASEDIR.'images/');
				//dodajemy kategorię do bazy
				$result = $database -> ExecuteQuery(sprintf(
					"insert into kategoria(nazwa_kat, nick, opis, zdjecie) values ('%s', '%s', '%s', '%s')",
					$database -> EscapeString($_POST['name']), 
					$_SESSION['nick'], 
					$database -> EscapeString($_POST['desc']), $image));
				if($result -> ErrorOccured())
				{	//w razie niepowodzenia usuwamy załadowany obrazek
					$view -> main[] = new Text($result -> GetError());
					if($image != "" && file_exists(BASEDIR.'images/'.$image))
						unlink(BASEDIR.'images/'.$image);
				}
				else
				{
					$showForm = false;
					$view -> main[] = new Text("Kategoria została dodana");
				}
			}
			catch(Exception $ex)
			{
				$view -> main[] = new Text($ex -> GetMessage());
			}
		}
	}
	else
	{
		$showForm = false;
		$view -> main[] = new Text("Nie masz uprawnień do wykonania tej akcji");
	}
	if($showForm)
	{	//a tu się nic nie dzieje
		$data = new CategoryData();
		$data -> name = isset($_POST['name']) ? $_POST['name'] : "";
		$data -> desc = isset($_POST['desc']) ? $_POST['desc'] : "";
		$data -> image = isset($_POST['image']) ? $_POST['image'] : "";
		
		$view -> main[] = CreateCategoryEditForm($data, "Stwórz kategorię", "index.php?action=addcategory", true);
		$view -> main[] = new Text("* - pole wymagane");
	}
	return $view;
	
}
