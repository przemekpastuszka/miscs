<?php

/*
Przemysław Pastuszka
2010

Edycja trasy
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
	$showForm = true;
	if($_SESSION['rank'] == 'uzytkownik' || $_SESSION['rank'] == 'administrator')
	{
		//procedura pobrania punktów zaznaczonych przez użytkownika (patrz: 'edittrail')
		if(isset($_POST['points']))
		{
			$q = explode(",", $_POST['points']);
			$p = array();
			foreach($q as $el)
				$p[] = floatval($el);
			$n = count($p);
			for($i = 0; $i < $n; $i += 2)
				$points[] = new Point($p[$i], $p[$i + 1]);
		}
		if(isset($_GET['ok']))
		{
			$showForm = false;
			try
			{ 	//tu też nic nowego, w stosunku do edittrail, się nie dzieje
				$image = LoadImage('image', 204800, BASEDIR.'images/');
				$res = $database -> ExecuteQuery(sprintf("select * from trasa where id_trasa = '%s'", 
					$database -> EscapeString($_GET['id'])));
				$trail = $res -> FetchAssoc();
				if($image == "")
					$image = $trail['zdjecie'];
				else
					if($trail['zdjecie'] != "")
						unlink(BASEDIR."images/".$trail['zdjecie']);
						
				$result = $database -> ExecuteQuery(sprintf(
					"update trasa_modyfikacja set nazwa_kat ='%s', opis='%s', zdjecie='%s'",
					$database -> EscapeString($_POST['cat']),
					$database -> EscapeString($_POST['desc']), $image));
				if($result -> ErrorOccured())
				{
					$view -> main[] = new Text($result -> GetError());
					if($image != "" && file_exists(BASEDIR.'images/'.$image))
						unlink(BASEDIR.'images/'.$image);
				}
				else
				{
					$id = $trail['id_trasa'];
					$database -> ExecuteQuery(sprintf("delete from punkt where id_trasa = '%s'", $id));
					$i = 1;
					foreach($points as $point)
					{
						$database -> ExecuteQuery(sprintf("insert into punkt(id_trasa, numer, szerokosc, dlugosc) values
						('%s', '%s', '%s', '%s')", $id, $i, $point -> x, $point -> y));
						++$i;
					}
					$view -> main[] = new Text("Trasa została zedytowana");
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
		$result = $database -> ExecuteQuery("select * from trasa where id_trasa='".
			$database -> EscapeString($_GET['id'])."'");
		if($result -> ErrorOccured() || $result -> RowsAffected() <= 0)
			$view -> main[] = new Text("Trasa o podanym id nie istnieje");
		else
		{
			$trail = $result -> FetchAssoc();
			$data = new TrailData();
			$data -> desc = $trail['opis'];
			$data -> category = $trail['nazwa_kat'];
			
			$result = $database -> ExecuteQuery(sprintf("select dlugosc, szerokosc from punkt where id_trasa = '%s' order by numer",
				$trail['id_trasa']));
			while($p = $result -> FetchAssoc())
				$points[] = new Point($p['szerokosc'], $p['dlugosc']);
	
			$view -> main[] = new Title("Edytuj trasę");
			$view = AddGoogleMap($view, $points, true);
			$view -> main[] = CreateTrailEditForm($data, "index.php?action=edittrail&ok=true&id=".$trail['id_trasa'], false, $database);
		}
	}
	return $view;	
}
