<?php
/*
Przemysław Pastuszka
2010

Interfejs bazy danych i rezultatu zapytania
Dzięki opakowaniu bazy w interfejs możemy łatwo dodawać implementacje różnych baz, które wykorzystują SQL
*/


interface Database
{
	public function Connect($host, $port, $dbname, $user, $password);
	public function Disconnect();
	public function ExecuteQuery($query);
	public function LastError();
	public function EscapeString($string);
}

interface QueryResult
{
	public function ErrorOccured();
	public function GetError();
	public function RowsAffected();
	public function FetchAssoc();
}

?>
