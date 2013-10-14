<?php

require_once(BASEDIR."includes/view/view.php");

/*
Przemysław Pastuszka
2010
*/

function SetHeader(View $view)
{
	$view -> header = new Header();
	$view -> header -> charset = "utf-8";
	$view -> header -> title = "Na rower - wycieczki rowerowe";
	$view -> header -> language = "pl";
	$view -> header -> author = "Przemysław Pastuszka";
	$view -> header -> description = "Serwis wspomagający organizację wycieczek rowerowych";
	$view -> header -> keywords = "rower, wycieczka, trasa, krajobraz, przyroda, czas, wolny";
	
	return $view;
}

?>
