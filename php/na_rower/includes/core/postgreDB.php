<?php

require_once(BASEDIR.'includes/core/database.php');

/*
Przemysław Pastuszka
2010

Implementacja interfejsu bazy danych i wyniku zapytania dla bazy PostgreSQL
*/

class PGQuery implements QueryResult
{
	private $error;
	private $done;
	private $result;
	
	public function __construct($result, $done, $error)
	{
		$this -> result = $result;
		$this -> done = $done;
		$this -> error = $error;
	}
	
	public function ErrorOccured()
	{
		return $this -> done;
	}
	
	public function GetError()
	{
		return $this -> error;
	}
	
	public function RowsAffected()
	{
		return pg_num_rows($this -> result);
	}
	
	public function FetchAssoc()
	{
		return pg_fetch_assoc($this -> result);
	}
}

class PostgreDB implements Database
{
	private $connection;
	
	public function Connect($host, $port, $dbname, $user, $password)
	{
		$this -> connection = pg_connect("host=$host port=$port dbname=$dbname user=$user password=$password");
		if(!$this -> connection)
			return false;
		return true;
	}
	public function Disconnect()
	{
		return pg_close($this -> connection);
	}
	public function ExecuteQuery($query)
	{
		$result = pg_query($this -> connection, $query);
		$errorOccured = $result ? false : true;
		$error = "";
		if($errorOccured)
			$error = pg_last_error($this -> connection);
		return new PGQuery($result, $errorOccured, $error);
	}
	
	public function LastError()
	{
		return pg_last_error($this -> connection);
	}
	
	public function EscapeString($string)
	{
		//pozbywamy się zagrożenia SQL Injection i XSS w jednym kroku
		return pg_escape_string($this -> connection, htmlspecialchars($string));
	}
}

?>
