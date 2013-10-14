<?php

/*
Przemysław Pastuszka
2010

Wyświetla kategorię o podanym id i wszystkie należące do niej trasy
*/

require_once(BASEDIR."includes/view/elements.php");
require_once(BASEDIR."includes/view/view.php");
require_once(BASEDIR."includes/core/database.php");
require_once(BASEDIR."includes/utilities/fillWithNewlines.php");

function SetMain(View $view, Database $database)
{
	$view -> main = array();
	$view -> main[] = new Title("Kategoria ".$_GET['name']);
	
	$result = $database -> ExecuteQuery(sprintf("select * from kategoria where nazwa_kat='%s'",
		$database -> EscapeString($_GET['name'])));
	if($result -> ErrorOccured() || $result -> RowsAffected() <= 0)
		$view -> main[] = new Text("Brak kategorii o podanej nazwie");
	else
	{
		/**WYŚWIETLENIE KATEGORII**/
		if($_SESSION['rank'] == 'uzytkownik' || $_SESSION['rank'] == 'administrator')
			$view -> main[] = new Link("Dodaj trasę w tej kategorii", "index.php?action=addtrail&name=".$_GET['name'], true);
		$actions = array();
		$row = $result -> FetchAssoc();
		if($_SESSION['rank'] == 'administrator')
		{
			$actions[] = new Link("Edytuj&nbsp;&nbsp;&nbsp;", BASEURL."index.php?action=editcategory&name=".$row['nazwa_kat']);
			$actions[] = new Link("Usuń", BASEURL."index.php?action=deletecategory&name=".$row['nazwa_kat']);
		}
		$elements = array();
		if($row['zdjecie'] != "")
		{
			$elements[] = new Image(BASEDIR."images/".$row['zdjecie'], 150, 150);
			$elements[] = new Text(FillWithNewlines(nl2br($row['opis']), 9));
		}
		else
				$elements[] = new Text(nl2br($row['opis']));
				
		$view -> main[] = new Post($row['nazwa_kat'], 
			new Link($row['nick'], BASEURL."index.php?action=profile&nick=".$row['nick']),
			$elements,
			"",
			"",
			$actions);
			
		/**WYŚWIETLENIE TRAS**/	
		$view -> main[] = new Title("Trasy w tej kategorii:");
		$result = $database -> ExecuteQuery(sprintf("select * from trasa where nazwa_kat='%s' order by nazwa",
			$database -> EscapeString($_GET['name'])));
		if($result -> ErrorOccured() || $result -> RowsAffected() <= 0)
			$view -> main[] = new Text("Brak tras w tej kategorii");
		else
		{
			while($row = $result -> FetchAssoc())
			{
				$actions = array();
				if($_SESSION['rank'] == 'administrator')
				{
					$actions[] = new Link("Edytuj&nbsp;&nbsp;&nbsp;", BASEURL."index.php?action=edittrail&id=".$row['id_trasa']);
					$actions[] = new Link("Usuń", BASEURL."index.php?action=deletetrail&id=".$row['id_trasa']);
				}	
				$view -> main[] = new Post($row['nazwa'], 
					new Link($row['nick'], BASEURL."index.php?action=profile&nick=".$row['nick']),
					array(new Text(nl2br($row['opis']))),
					date("d-m-Y", strtotime($row['data_dodania'])),
					new Link("Pokaż trasę", BASEURL."index.php?action=trail&id=".$row['id_trasa']),
					$actions);
			}
		}
	}
	return $view;
}
