<?php

/*
Przemysław Pastuszka
2010

Logowanie użytkownika
*/

require_once(BASEDIR."includes/view/elements.php");
require_once(BASEDIR."includes/view/view.php");
require_once(BASEDIR."includes/core/database.php");

function SetMain(View $view, Database $database)
{
	$view -> main = array();
	
	if(isset($_POST['nick'])) //przesłano formularz
	{	
		//sprawdzamy poprawność hasła
		$result = $database -> ExecuteQuery("select sprawdz_haslo('".
			$database -> EscapeString($_POST['nick'])."', '".
			sha1($_POST['password'])."')");
			
		$res = 'f';
		if(!$result -> ErrorOccured())
		{
			$row = $result -> FetchAssoc();
			$res = $row['sprawdz_haslo'];	
		}
		
		if($res != 't')
			$view -> main[] = new Text("Błędna nazwa użytkownika lub hasło");
		else
		{
			$_SESSION['nick'] = $database -> EscapeString($_POST['nick']);
			if($_SESSION['nick'] == 'admin') //użytkownik o nicku 'admin' jest administratorem
				$_SESSION['rank'] = 'administrator';
			else
				$_SESSION['rank'] = 'uzytkownik';
			$database -> Disconnect(); //będziemy przeładowywać stronę, więc rozłączamy się z bazą i zapisujemy sesję
			session_write_close();
			header('Location: '.BASEURL.'index.php');
		}
	}
	

	$nick = isset($_POST['nick']) ? $_POST['nick'] : "";
	
	$view -> main[] = new Form("Logowanie", "index.php?action=login", array(
		new InputText("nick", "Nick:", $nick, 15),
		new InputText("password", "Hasło:", "", 50, "password")
		));
		
		
	$view -> main[] = new Text("Nie masz jeszcze konta w serwisie?");/*<br/>
		<a href='".BASEURL."index.php?action=register'>Zarejestruj się!</a>");	*/
	$link = new Link("Zarejestruj się!", BASEURL."index.php?action=register");
	$link -> shifted = true;
	$view -> main[] = $link;
	return $view;
}

?>
