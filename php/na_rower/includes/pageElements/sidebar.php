<?php

require_once(BASEDIR."includes/view/elements.php");
require_once(BASEDIR."includes/view/view.php");
require_once(BASEDIR."includes/core/database.php");

/*
Przemysław Pastuszka
2010
*/

function SetSidebar(View $view, Database $database)
{
	global $debug;
	
	//najnowsze trasy
	$result = $database -> ExecuteQuery("select id_trasa, nazwa from trasa order by data_dodania desc limit 5");
	if($result -> ErrorOccured() && $debug)
		echo $result -> GetError();
	
	if(!$result -> ErrorOccured())
	{
		$content = array();
		while($row = $result -> FetchAssoc())
			$content[] = new Link($row['nazwa'], BASEURL."index.php?action=trail&id=".$row['id_trasa']);
		$view -> sidebar[] = new SidebarElement("Najnowsze trasy", new SideMenu($content));
	}
	
	
	//najbliższe wycieczki
	$result = $database -> ExecuteQuery("select id_wycieczka, nazwa, data from wycieczka join trasa using(id_trasa) where 
		data > now() order by data limit 5");
	if($result -> ErrorOccured() && $debug)
		echo $result -> GetError();
		
	if(!$result -> ErrorOccured())
	{
		$content = array();
		while($row = $result -> FetchAssoc())
			$content[] = new Link($row['nazwa'], BASEURL."index.php?action=trip&id=".$row['id_wycieczka']);
		$view -> sidebar[] = new SidebarElement("Najbliższe wycieczki", new SideMenu($content));
	}
	return $view;
}

?>
