<?php

/*
Przemysław Pastuszka
2010

Wyświetla wszystkie wycieczki
*/

require_once(BASEDIR."includes/view/elements.php");
require_once(BASEDIR."includes/view/view.php");
require_once(BASEDIR."includes/core/database.php");

function SetMain(View $view, Database $database)
{
	$view -> main = array();
	
	if($_SESSION['rank'] == 'uzytkownik' || $_SESSION['rank'] == 'administrator')
		$view -> main[] = new Link("Dodaj wycieczkę", BASEURL."index.php?action=addtrip", true);
	$result = $database -> ExecuteQuery("select wycieczka.*, nazwa from wycieczka join trasa using (id_trasa) where data > now() order by data");
	if($result -> ErrorOccured() || $result -> RowsAffected() <= 0)
		$view -> main[] = new Text("Brak aktywnych wycieczek");
	else
	{
		$elements = array();
		$header = array(new Text("Kiedy"), new Text("Trasa"), new Text("Więcej"), new Text("Edytuj"));
		while($row = $result -> FetchAssoc())
		{
			$currEl = array();
			$currEl[] = new Text(date("d-m-Y", strtotime($row['data'])));
			$currEl[] = new Link($row['nazwa'], "index.php?action=trail&id=".$row['id_trasa'], true);
			$currEl[] = new Link("Więcej", "index.php?action=trip&id=".$row['id_wycieczka'], true);
			if($_SESSION['rank'] == 'administrator' || ($_SESSION['rank'] == 'uzytkownik' && $_SESSION['nick'] == $row['nick']))
				$currEl[] = new Link("Edytuj / Usuń", "index.php?action=edittrip&id=".$row['id_wycieczka'], true);
			$elements[] = $currEl;
		}
		$view -> main[] = new Table($elements, $header);
	}
	return $view;	
}
