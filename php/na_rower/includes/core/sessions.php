<?php
/*
Przemysław Pastuszka
2010

Tutaj wszystkie funkcje zarządzajace sesją
*/

//tworzy i/lub sprawdza poprawność sesji
function InitalizeAndCheckSession()
{
	$expireTime = 300;	//można ustawiać wedle życzenia
	
	if(!session_start())
		return false;

	//sprawdzamy czy sesja nie wygasła
	if((!isset($_SESSION['expireTime'])) || $_SESSION['expireTime'] < time())
	{
		if(!session_regenerate_id(true))
			return false;
		$_SESSION['rank'] = 'gosc';
		$_SESSION['ip'] = $_SERVER['REMOTE_ADDR'];
	}

	//albo jakiś zły człowiek nie podjął próby przejęcia sesji
	if($_SESSION['ip'] != $_SERVER['REMOTE_ADDR'])
		throw new Exception('Błędny adres IP');

	$_SESSION['expireTime'] = time() + $expireTime;
		
	return true;
}

function DestroySession()
{
	session_unset();
	session_destroy();
}
?>
