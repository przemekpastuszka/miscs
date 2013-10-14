<?php

require_once(BASEDIR."includes/view/view.php");

/*
Przemysław Pastuszka
2010
*/

//ustawia menu
function SetMenu(View $view)
{
	$view -> menu = array(
		//"index" => new Link("Aktualności", BASEURL."index.php"),
		"trails" => new Link("Trasy rowerowe", BASEURL."index.php?action=categories"),
		"trips" => new Link("Wycieczki", BASEURL."index.php?action=trips"));
	
	if($_SESSION['rank'] == 'uzytkownik' || $_SESSION['rank'] == 'administrator')
	{
		$view -> menu['profile'] = new Link("Profil", BASEURL."index.php?action=profile&nick=".$_SESSION['nick']);
		$view -> menu['logout'] = new Link("Wyloguj", BASEURL."index.php?action=logout");
	}
	else
		$view -> menu['login'] = new Link("Zaloguj", BASEURL."index.php?action=login");
	
	
	if(isset($_GET['action']) && isset($view -> menu[$_GET['action']]) 
			&& in_array($_GET['action'], array(/*'index',*/ 'categories', 'trips', 'login', 'profile')))
		$view -> menu[$_GET['action']] -> highlighted = true;
	
	return $view;
}

?>
