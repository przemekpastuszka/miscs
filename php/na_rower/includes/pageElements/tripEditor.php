<?php

require_once(BASEDIR."includes/view/elements.php");

/*
Przemysław Pastuszka
2010
*/

class TripData
{
	public $trail;
	public $desc;
	public $date;
}

function CreateTripEditForm(TripData $data, $title, $action, $database)
{
	$elements = array();
	
	//trzeba wypisać trasy, dzieląc je jednocześnie na kategorie
	$result = $database -> ExecuteQuery("select id_trasa, nazwa_kat, nazwa from trasa order by nazwa_kat, nazwa");
	$last = "";
	$options = array();
	if(!$result -> ErrorOccured())
	{
		while($row = $result -> FetchAssoc())
		{
			if($row['nazwa_kat'] != $last)
			{
				$last = $row['nazwa_kat'];
				$options[] = new InputOption("", $last, false, true);
			}
			if($data -> trail == $row['id_trasa'])
				$options[] = new InputOption($row['id_trasa'], "&nbsp;&nbsp;&nbsp;&nbsp;".$row['nazwa'], true);
			else
				$options[] = new InputOption($row['id_trasa'], "&nbsp;&nbsp;&nbsp;&nbsp;".$row['nazwa']);
		}
	}
	$elements[] = new InputSelect("trail", "Trasa:", $options);
	$elements[] = new InputArea("desc", "Opis: ", $data -> desc);
	$elements[] = new InputText("date", "Data (dd-mm-rrrr):", $data -> date, 10);
	
	return new Form($title, $action, $elements);
}
