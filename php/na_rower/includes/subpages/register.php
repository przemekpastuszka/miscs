<?php

/*
Przemysław Pastuszka
2010

Rejestracja użytkownika
*/

require_once(BASEDIR."includes/view/elements.php");
require_once(BASEDIR."includes/view/view.php");
require_once(BASEDIR."includes/core/database.php");
require_once(BASEDIR."includes/pageElements/profileEditor.php");

function SetMain(View $view, Database $database)
{
	$view -> main = array();
	
	$showForm = true;
	
	if(isset($_POST['nick']))
	{
		//sprawdzenie czy użytkownik dobrze powtórzył hasło
		if($_POST['password'] != $_POST['passwordRepeat'])
			$view -> main[] = new Text("Błędnie powtórzone hasło!");
		else if($_POST['password'] == "")
			$view -> main[] = new Text("Hasło nie może być puste!");
		else if($_POST['nick'] == "")
			$view -> main[] = new Text("Nazwa użytkownika nie może być pusta!");
		else
		{
			$result = $database -> ExecuteQuery(
				sprintf("insert into uzytkownik values('%s', '%s', '%s', '%s', '%s', '%s')",
				$database -> EscapeString($_POST['nick']), 
				$database -> EscapeString($_POST['name']), 
				$database -> EscapeString($_POST['surname']), 
				$database -> EscapeString($_POST['desc']), 
				$database -> EscapeString($_POST['bicycle']), 
				$database -> EscapeString(sha1($_POST['password']))));
			if($result -> ErrorOccured())
				$view -> main[] = new Text($result -> GetError());
			else
			{
				$view -> main[] = new Text("Twoje konto zostało utworzone. Możesz się teraz zalogować.");
				$showForm = false;
			}
		}
		
	}
	
	if($showForm)
	{
		$data = new ProfileData();
		$data -> nick = isset($_POST['nick']) ? $_POST['nick'] : "";
		$data -> name = isset($_POST['name']) ? $_POST['name'] : "";
		$data -> surname = isset($_POST['surname']) ? $_POST['surname'] : "";
		$data -> bicycle = isset($_POST['bicycle']) ? $_POST['bicycle'] : "";
		$data -> desc = isset($_POST['desc']) ? $_POST['desc'] : "";
	
		$view -> main[] = CreateProfileEditForm($data, "Rejestracja", "index.php?action=register", true);
		$view -> main[] = new Text("* - pole wymagane");
	}
	return $view;
}

?>
