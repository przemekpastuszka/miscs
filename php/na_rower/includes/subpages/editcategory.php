<?php

/*
Przemysław Pastuszka
2010

Edycja kategorii
*/

require_once(BASEDIR."includes/view/elements.php");
require_once(BASEDIR."includes/view/view.php");
require_once(BASEDIR."includes/core/database.php");
require_once(BASEDIR."includes/pageElements/categoryEditor.php");
require_once(BASEDIR."includes/utilities/imageLoader.php");

function SetMain(View $view, Database $database)
{

	$view -> main = array();
	$showForm = true;
	//sprawdzenie uprawnień
	if(isset($_GET['name']) && $_SESSION['rank'] == 'administrator')
	{
		if(isset($_GET['ok']))
		{
			try
			{ 
				$image = LoadImage('image', 204800, BASEDIR.'images/');
				$res = $database -> ExecuteQuery(sprintf("select zdjecie from kategoria where nazwa_kat = '%s'", 
					$database -> EscapeString($_GET['name'])));
				$cat = $res -> FetchAssoc();
				
				//sprawdzamy czy załadowano nowe zdjęcie (jeśli nie to pozostawiamy poprzednie)
				if($image == "")
					$image = $cat['zdjecie'];
				else
					if($cat['zdjecie'] != "")
						unlink(BASEDIR."images/".$cat['zdjecie']);
				$result = $database -> ExecuteQuery(sprintf(
					"update kategoria_modyfikacja set opis='%s', zdjecie='%s' where nazwa_kat='%s'",
					$database -> EscapeString($_POST['desc']), 
					$image, 
					$database -> EscapeString($_GET['name'])));
				if($result -> ErrorOccured())
				{
					$view -> main[] = new Text($result -> GetError());
					if($image != "" && file_exists(BASEDIR.'images/'.$image))
						unlink(BASEDIR.'images/'.$image);
				}
				else
				{
					$showForm = false;
					$view -> main[] = new Text("Kategoria została zmodyfikowana");
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
	{
		$result = $database -> ExecuteQuery("select * from kategoria where nazwa_kat='".
			$database -> EscapeString($_GET['name'])."'");
		if($result -> ErrorOccured() || $result -> RowsAffected() <= 0)
			$view -> main[] = new Text("Kategoria o podanej nazwie nie istnieje");
		else
		{
			$cat = $result -> FetchAssoc();
			$data = new CategoryData();
			
			$data -> desc = $cat['opis'];
		
			$view -> main[] = CreateCategoryEditForm($data, "Edytuj kategorię", "index.php?action=editcategory&ok=true&name=".$_GET['name'], false);
		}
	}
	return $view;
	
}
