<?php

/*
Przemysław Pastuszka
2010

Wyświetlenie profilu użytkownika
*/

require_once(BASEDIR."includes/view/elements.php");
require_once(BASEDIR."includes/view/view.php");
require_once(BASEDIR."includes/core/database.php");

function SetMain(View $view, Database $database)
{
	$view -> main = array();
	$view -> main[] = new Title("Profil użytkownika ".$_GET['nick']);
	
	if(isset($_GET['nick']) && ($_SESSION['rank'] == 'uzytkownik' || $_SESSION['rank'] == 'administrator'))
	{
		$result = $database -> ExecuteQuery(sprintf("select * from uzytkownik_bez_hasla where nick = '%s'", 
			$database -> EscapeString($_GET['nick'])));
		if($result -> ErrorOccured() || $result -> RowsAffected() <= 0)
			$view -> main[] = new Text("Użytkownik o podanym nicku nie istnieje");
		else
		{
			$user = $result -> FetchAssoc();
			$content = array();
			$content[] = array(new Text("Nick"), new Text($user['nick']));
			$content[] = array(new Text("Imię"), new Text($user['imie']));
			$content[] = array(new Text("Nazwisko"), new Text($user['nazwisko']));
			$content[] = array(new Text("Opis"), new Text(nl2br($user['opis'])));
			$content[] = array(new Text("Rower"), new Text($user['rodzaj_roweru']));
			$table = new Table($content);
			$view -> main[] = $table;
			
			$linkA = new Link("Edytuj profil", BASEURL."index.php?action=editprofile&nick=".$user['nick']);
			$linkB = new Link("Usuń profil", BASEURL."index.php?action=deleteprofile&nick=".$user['nick']);
			$linkA -> shifted = $linkB -> shifted = true;
			
			if($_SESSION['nick'] == $user['nick'] || $_SESSION['rank'] == 'administrator')
				$view -> main[] = $linkA;
			if($_SESSION['rank'] == 'administrator' && $user['nick'] != 'admin')
				$view -> main[] = $linkB;
			
		}
	}
	else
		$view -> main[] = new Text("Tylko zalogowani użytkownicy mogą oglądać ten profil");
	
	return $view;	
}

?>
