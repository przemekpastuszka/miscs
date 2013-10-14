<?php

/*
Przemysław Pastuszka
2010

Funkcja łącząca z bazą danych (wykorzystuje dane zawarte w db/data.php
*/

require_once(BASEDIR."db/data.php");

function ConnectToDB(Database $db)
{
	$dbdata = DBData :: $dbdata;
	if(!DBData :: IsRankOK($_SESSION['rank'])) //sprawdzenie czy logujemy się przy użyciu poprawnej rangi
		throw new Exception('Niepoprawna ranga użytkownika');

	return $db -> Connect($dbdata['host'], $dbdata['port'], $dbdata['dbname'], $_SESSION['rank'], DBData :: GetPassword($_SESSION['rank']));
}

?>
