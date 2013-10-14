<?php

/*
Przemysław Pastuszka
2010
*/

//klasa, która przechowuje informacje o dostępie do bazy danych
//należy zmienić odpowiednie wartości
class DBData
{
	public static $dbdata = array("host" => "localhost", "port" => 5432, "dbname" => "narower");
	private static $possibleRanks = array("gosc", "uzytkownik", "administrator");
	private static $passwords = array("gosc" => "test", "uzytkownik" => "test", "administrator" => "test");
	
	public static function GetPassword($nick)
	{
		return DBData :: $passwords[$nick];
	}

	public static function IsRankOk($rank)
	{
		return in_array($rank, DBData :: $possibleRanks);
	}
}
?>
