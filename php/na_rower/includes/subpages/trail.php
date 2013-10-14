<?php

/*
Przemysław Pastuszka
2010

Wyświetlenie trasy
*/

require_once(BASEDIR."includes/view/elements.php");
require_once(BASEDIR."includes/view/view.php");
require_once(BASEDIR."includes/core/database.php");
require_once(BASEDIR."includes/utilities/googlemap.php");

function SetMain(View $view, Database $database)
{
	$view -> main = array();
	$result = $database -> ExecuteQuery(sprintf("select * from trasa where id_trasa = '%s'",
		$database -> EscapeString($_GET['id'])));
	if($result -> ErrorOccured() || $result -> RowsAffected() <= 0)
		$view -> main[] = new Text("Brak trasy o podanym id");
	else
	{
		$trail = $result -> FetchAssoc();
		
		//jeśli użytkownik dodał ocenę
		if(isset($_POST['score'])  && ($_SESSION['rank'] == 'uzytkownik' || $_SESSION['rank'] == 'administrator'))
		{
			$result = $database -> ExecuteQuery(sprintf("insert into ocena(nick, id_trasa, kat_oceny, ocena) values ('%s', '%s', '%s', '%s')",
				$_SESSION['nick'],
				$trail['id_trasa'],
				$database -> EscapeString($_POST['score_cat']),
				$database -> EscapeString($_POST['score'])));
			if($result -> ErrorOccured())
				$view -> main[] = new Text($result -> GetError());
			else
				$view -> main[] = new Text("Oceniłeś trasę");
		}
		
		//użytkownik dodał komentarz
		if(isset($_POST['comment_txt']) && ($_SESSION['rank'] == 'uzytkownik' || $_SESSION['rank'] == 'administrator'))
		{
			$result = $database -> ExecuteQuery(sprintf("insert into komentarz(id_trasa, nick, tresc) values
				('%s', '%s', '%s')",
				$trail['id_trasa'],
				$_SESSION['nick'],
				$database -> EscapeString($_POST['comment_txt'])));
			if($result -> ErrorOccured())
				$view -> main[] = new Text($result -> GetError());
			else
				$view -> main[] = new Text("Komentarz został dodany");
		}
		
		//użytkownik dodał czas
		if(isset($_POST['time']) && ($_SESSION['rank'] == 'uzytkownik' || $_SESSION['rank'] == 'administrator'))
		{
			$result = $database -> ExecuteQuery(sprintf("insert into czas(id_trasa, nick, czas) values
				('%s', '%s', '%s')",
				$trail['id_trasa'],
				$_SESSION['nick'],
				$database -> EscapeString($_POST['time'])));
			if($result -> ErrorOccured())
				$view -> main[] = new Text("Podano czas w niepoprawnym formacie");
			else
				$view -> main[] = new Text("Twój czas został dodany");
		}
		
		//pobieramy z bazy punkty tworzące mapę
		$result = $database -> ExecuteQuery(sprintf("select dlugosc, szerokosc from punkt where id_trasa = '%s' order by numer",
			$trail['id_trasa']));
		$points = array();
		while($p = $result -> FetchAssoc())
			$points[] = new Point($p['szerokosc'], $p['dlugosc']);
	
		$view -> main[] = new Title($trail['nazwa']);
		$view = AddGoogleMap($view, $points, false);
		
		//wyświetlamy opis trasy
		$content = array();
		$content[] = array(new Text("Autor:"), new Link($trail['nick'], BASEDIR."index.php?action=profile&nick=".$trail['nick']));
		$content[] = array(new Text("Opis:"), new Text(nl2br($trail['opis'])));
		$content[] = array(new Text("Data dodania:"), new Text(date("d-m-Y", strtotime($trail['data_dodania']))));
		if($trail['zdjecie'] != "")
			$content[] = array(new Text("Zdjęcie:"), new Image(BASEDIR."images/".$trail['zdjecie'], 0, 0));
		$view -> main[] = new Table($content);
		if($_SESSION['rank'] == 'administrator')
		{
			$view -> main[] = new Link("Edytuj", BASEURL."index.php?action=edittrail&id=".$trail['id_trasa'], true);
			$view -> main[] = new Link("Usuń", BASEURL."index.php?action=deletetrail&id=".$trail['id_trasa'], true);
		}
		
		//wyświetlamy średnie oceny
		$content = array();
		$result = $database -> ExecuteQuery(sprintf("select avg(ocena), kat_oceny from ocena where id_trasa = '%s' group by kat_oceny",
			$trail['id_trasa']));
		$view -> main[] = new Title("Średnie oceny trasy");
		while($notes = $result -> FetchAssoc())
		{
			switch($notes['kat_oceny'])
			{
				case 'stt':
					$content[] = array(new Text("Stopień trudności trasy"), new Text($notes['avg']));
					break;
				case 'tpr':
					$content[] = array(new Text("Trasa przyjazna rowerzyście"), new Text($notes['avg']));
					break;
				case 'we':
					$content[] = array(new Text("Walory estetyczne"), new Text($notes['avg']));
					break;
				case 'oo':
					$content[] = array(new Text("Ocena ogólna"), new Text($notes['avg']));
					break;
			}
		}
		$view -> main[] = new Table($content);
		
		
		//formularz oceny trasy
		if($_SESSION['rank'] == 'uzytkownik' || $_SESSION['rank'] == 'administrator')
		{
			$scores = array();
			for($i = 1; $i <= 6; ++$i)
				$scores[] = new InputOption($i, $i);
			$scoreCats = array(
				new InputOption("stt", "Stopień trudności trasy"),
				new InputOption("tpr", "Trasa przyjazna rowerzyście"),
				new InputOption("we", "Walory estetyczne"),
				new InputOption("oo", "Ocena ogólna"));
			$scoreElems = array(new InputSelect("score_cat", "Kategoria oceny:", $scoreCats),
				new InputSelect("score", "Ocena:", $scores));
			$view -> main[] = new Form("Oceń trasę w wybranych kategoriach", "index.php?action=trail&id=".$trail['id_trasa'],
				$scoreElems);
		}
		
		//wyświetlamy czasy na tej trasie	
		$content = array();
		$result = $database -> ExecuteQuery(sprintf("select nick, czas from czas where id_trasa = '%s' order by czas",
			$trail['id_trasa']));
		$view -> main[] = new Title("Czasy przejazdu użytkowników");
		while($time = $result -> FetchAssoc())
		{
			$content[] = array(new Link($time['nick'], BASEDIR."index.php?action=profile&nick=".$time['nick']),
				new Text($time['czas']));
		}
		$view -> main[] = new Table($content, array(new Text("Nick"), new Text("Czas")));
		

		if($_SESSION['rank'] == 'uzytkownik' || $_SESSION['rank'] == 'administrator')
		{
			//formularz pozwalający na dodanie czasu przejazdu
			$view -> main[] = new Form("Dodaj własny czas przejazdu", "index.php?action=trail&id=".$trail['id_trasa'],
				array(new InputText("time", "Czas (hh:mm:ss):")));
		
			//wyświetlenie komentarzy
			$view -> main[] = new Title("Komentarze:");
			$result = $database -> ExecuteQuery(sprintf("select * from komentarz where id_trasa ='%s' order by data desc", $trail['id_trasa']));
			while($comment = $result -> FetchAssoc())
			{
				$view -> main[] = new Post("Komentarz",
					new Link($comment['nick'], BASEURL."index.php?action=profile&nick=".$comment['nick']),
					array(new Text(nl2br($comment['tresc']))),
					date("d-m-Y H:i:s", strtotime($comment['data'])),
					"", array());
			}
			
			//formularz dodania komentarza
			$elements = array(new InputArea("comment_txt", "Treść komentarza", ""));
			$view -> main[] = new Form("Dodaj komentarz", "index.php?action=trail&id=".$trail['id_trasa'], $elements, "");
		}
		else
			$view -> main[] = new Text("Musisz być zalogowany by móc oceniać trasę i oglądać komentarze");
	}
	return $view;
	
}
