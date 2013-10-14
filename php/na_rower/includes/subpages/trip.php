<?php

/*
Przemysław Pastuszka
2010

Wyświetl wycieczkę
*/

require_once(BASEDIR."includes/view/elements.php");
require_once(BASEDIR."includes/view/view.php");
require_once(BASEDIR."includes/core/database.php");

function SetMain(View $view, Database $database)
{
	$view -> main = array();
		
	$result = $database -> ExecuteQuery(sprintf("select wycieczka.*, nazwa from wycieczka join trasa using (id_trasa) where id_wycieczka = '%s'",
		$database -> EscapeString($_GET['id'])));
	if($result -> ErrorOccured() || $result -> RowsAffected() <= 0)
		$view -> main[] = new Text("Brak wycieczki o podanym id");
	else
	{
		$trip = $result -> FetchAssoc();
		
		//użytkownik zapisał się na wycieczkę
		if(isset($_POST['join']) && ($_SESSION['rank'] == 'uzytkownik' || $_SESSION['rank'] == 'administrator'))
		{
			$result = $database -> ExecuteQuery(sprintf("insert into zapis values('%s', '%s')", $trip['id_wycieczka'], $_SESSION['nick']));
			if($result -> ErrorOccured())
				$view -> main[] = new Text($result -> GetError());
		}
		
		//użytkownik wypisał się z wycieczki
		if(isset($_POST['disjoin']) && ($_SESSION['rank'] == 'uzytkownik' || $_SESSION['rank'] == 'administrator'))
		{
			$result = $database -> ExecuteQuery(sprintf("delete from zapis where id_wycieczka = '%s' and nick = '%s'", $trip['id_wycieczka'], $_SESSION['nick']));
			if($result -> ErrorOccured())
				$view -> main[] = new Text($result -> GetError());
		}
		
		$view -> main[] = new Title("Wycieczka na trasie ".$trip['nazwa']);
		
		//opis wycieczki
		$elements = array();
		$elements[] = array(new Text("Organizator:"), new Link($trip['nick'], BASEURL."index.php?action=profile&nick=".$trip['nick'], true));
		$elements[] = array(new Text("Trasa:"), new Link($trip['nazwa'], "index.php?action=trail&id=".$trip['id_trasa'], true));
		$elements[] = array(new Text("Data wycieczki:"), new Text(date("d-m-Y", strtotime($trip['data']))));
		$elements[] = array(new Text("Opis:"), new Text(nl2br($trip['opis'])));
		$view -> main[] = new Table($elements);
		
		if($_SESSION['rank'] == 'uzytkownik' || $_SESSION['rank'] == 'administrator')
		{
			//wyświetlenie zapisanych użytkowników
			$result = $database -> ExecuteQuery(sprintf("select nick from zapis where id_wycieczka='%s'", $trip['id_wycieczka']));
			$you = false;
			if($result -> ErrorOccured() || $result -> RowsAffected() <= 0)
				$view -> main[] = new Text("Nikt nie zapisał się jeszcze na tę wycieczkę");
			else
			{
				$members = array();
				while($user = $result -> FetchAssoc())
				{
					$members[] = array(new Link($user['nick'], BASEURL."index.php?action=profile&nick=".$user['nick'], true));
					if($user['nick'] == $_SESSION['nick'])
						$you = true;
				}
				$view -> main[] = new Table($members, array(new Text("Osoby zapisane na tę wycieczkę")));
			}
			if($you)
			{	//formularz wypisu
				$elem = array(new InputText("disjoin", "Wypisz się z tej wycieczki", "", 30, "hidden"));
				$view -> main[] = new Form("", BASEURL."index.php?action=trip&id=".$trip['id_wycieczka'], $elem);
			}
			else
			{	//formularz zapisu
				$elem = array(new InputText("join", "Zapisz się na tę wycieczkę", "", 30, "hidden"));
				$view -> main[] = new Form("", BASEURL."index.php?action=trip&id=".$trip['id_wycieczka'], $elem);

			}
		}
		
	}
	return $view;
}
