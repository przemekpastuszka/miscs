<?php


/*
Przemysław Pastuszka
2010

Tutaj znajduje się kod dodawania tras
*/

require_once(BASEDIR."includes/view/elements.php");
require_once(BASEDIR."includes/view/view.php");
require_once(BASEDIR."includes/core/database.php");
require_once(BASEDIR."includes/pageElements/trailEditor.php");
require_once(BASEDIR."includes/utilities/imageLoader.php");
require_once(BASEDIR."includes/utilities/googlemap.php");

function SetMain(View $view, Database $database)
{

	$view -> main = array();
	$points = array();
	$showForm = true; //czy pokazać formularz
	//dodawać trasę może tylko użytkownik bądź administrator
	if($_SESSION['rank'] == 'uzytkownik' || $_SESSION['rank'] == 'administrator')
	{
		$n = 0;
		//przesłano formularz (i zaznaczono punkty na mapie)
		if(isset($_POST['points']))
		{
			$q = explode(",", $_POST['points']); //punkty dostajemy jako string. Separatorem jest przecinek
			$p = array();
			foreach($q as $el)
				$p[] = floatval($el); //pobieramy wartość zmiennoprzecinkową
			$n = count($p);
			for($i = 0; $i < $n; $i += 2)
				$points[] = new Point($p[$i], $p[$i + 1]); //i tworzymy instancję klasy punkt
		}
		if(isset($_POST['name']) && $_POST['name'] != "" && $n >= 4)
		{
			try
			{	//ładuj obrazek
				$image = LoadImage('image', 204800, BASEDIR.'images/');
				$result = $database -> ExecuteQuery(sprintf(
					"insert into trasa(nazwa, nazwa_kat, nick, opis, zdjecie) values ('%s', '%s', '%s', '%s', '%s')",
					$database -> EscapeString($_POST['name']),
					$database -> EscapeString($_POST['cat']),
					$_SESSION['nick'], 
					$database -> EscapeString($_POST['desc']), $image));
				if($result -> ErrorOccured())
				{
					$view -> main[] = new Text($result -> GetError());
					if($image != "" && file_exists(BASEDIR.'images/'.$image))
						unlink(BASEDIR.'images/'.$image);
				}
				else
				{	//nie wystąpił żaden błąd - dodajemy punkty do bazy danych
					//ale najpierw trzeba dowiedzieć się jakie id otrzymała nasza nowa trasa
					$result = $database -> ExecuteQuery(sprintf("select id_trasa from trasa where nazwa = '%s'",
						$database -> EscapeString($_POST['name'])));
					$trail = $result -> FetchAssoc();
					$id = $trail['id_trasa'];
					$i = 1;
					foreach($points as $point)
					{
						$database -> ExecuteQuery(sprintf("insert into punkt(id_trasa, numer, szerokosc, dlugosc) values
						('%s', '%s', '%s', '%s')", $id, $i, $point -> x, $point -> y));
						++$i;
					}
					$showForm = false;
					$view -> main[] = new Text("Trasa została dodana");
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
		$data = new TrailData();
		$data -> name = isset($_POST['name']) ? $_POST['name'] : "";
		$data -> desc = isset($_POST['desc']) ? $_POST['desc'] : "";
		$cat = isset($_GET['name']) ? $_GET['name'] : "";
		$data -> category = isset($_POST['cat']) ? $_POST['cat'] : $cat;
	
	
		$view -> main[] = new Title("Stwórz trasę");
		$view = AddGoogleMap($view, $points, true); //o, tutaj dodajemy mapę do strony
		$view -> main[] = CreateTrailEditForm($data, "index.php?action=addtrail", true, $database);
		$view -> main[] = new Text("* - pole wymagane");
	}
	return $view;	
}
