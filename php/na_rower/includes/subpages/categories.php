<?php

/*
Przemysław Pastuszka
2010

Poniższy kod wyświetla wszystkie kategorie w porządku alfabetycznym
*/

require_once(BASEDIR."includes/view/elements.php");
require_once(BASEDIR."includes/view/view.php");
require_once(BASEDIR."includes/core/database.php");
require_once(BASEDIR."includes/utilities/fillWithNewlines.php");

function SetMain(View $view, Database $database)
{
	$view -> main = array();
	$view -> main[] = new Title("Kategorie tras rowerowych");
	
	//dodawać kategorie mogą użytkownicy i administrator
	if($_SESSION['rank'] == 'uzytkownik' || $_SESSION['rank'] == 'administrator')
		$view -> main[] = new Link("Dodaj kategorię", "index.php?action=addcategory", true);
	
	$result = $database -> ExecuteQuery("select * from kategoria order by nazwa_kat");
	if($result -> ErrorOccured() || $result -> RowsAffected() <= 0)
		$view -> main[] = new Text("Brak kategorii tras rowerowych");
	else
	{
		while($row = $result -> FetchAssoc())
		{
			$actions = array();
			//usuwać i edytować tylko administrator
			if($_SESSION['rank'] == 'administrator')
			{
				$actions[] = new Link("Edytuj&nbsp;&nbsp;&nbsp;", BASEURL."index.php?action=editcategory&name=".$row['nazwa_kat']);
				$actions[] = new Link("Usuń", BASEURL."index.php?action=deletecategory&name=".$row['nazwa_kat']);
			}
			
			//sprawdzamy czy trzeba wyświetlić zdjęcie
			$elements = array();
			if($row['zdjecie'] != "")
			{
				$elements[] = new Image(BASEDIR."images/".$row['zdjecie'], 150, 150);
				//żeby strona się nie "rozjechała", tekst zostaje dopełniony znakami nowej linii
				$elements[] = new Text(FillWithNewlines(nl2br($row['opis']), 9));
			}
			else
				$elements[] = new Text(nl2br($row['opis']));
			
			$view -> main[] = new Post($row['nazwa_kat'], 
				new Link($row['nick'], BASEURL."index.php?action=profile&nick=".$row['nick']),
				$elements,
				"",
				new Link("Pokaż trasy", BASEURL."index.php?action=category&name=".$row['nazwa_kat']),
				$actions);
		}
	}
	
	return $view;	
}
