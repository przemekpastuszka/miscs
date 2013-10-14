<?php

/*
Przemysław Pastuszka
2010

Tekst powitalny
*/

require_once(BASEDIR."includes/view/elements.php");
require_once(BASEDIR."includes/view/view.php");
require_once(BASEDIR."includes/core/database.php");

function SetMain(View $view, Database $database)
{
	$view -> main = array();
	$view -> main[] = new Title("Witaj w serwisie narower!");
	$view -> main[] = new Text("Witam"); //można zmienić
	return $view;
}

?>
