<?php

require_once(BASEDIR."includes/core/postgreDB.php");

/*
Przemysław Pastuszka
2010
*/

class DatabaseManager
{
	public static function GetDB()
	{
		return new PostgreDB(); //obsługa tylko jednej bazy została zaimplementowana, więc nic innego tu zdarzyć się nie może
	}
}

?>
