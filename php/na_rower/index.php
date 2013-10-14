<?php
/*
Przemysław Pastuszka
2010
*/
try
{
	//podstawowa konfiguracja
	require_once("config/config.php");

	//inicjalizacja sesji
	require_once(BASEDIR."includes/core/sessions.php");
	if(!InitalizeAndCheckSession())
		throw new Exception('Błąd w trakcie inicjalizacji sesji');
	
	//łączenie się z bazą danych
	require_once(BASEDIR."includes/core/dbManager.php");
	$database = DatabaseManager :: GetDB(); //pobierz bazę danych z managera
	require_once(BASEDIR."includes/core/dbConnect.php");
	if(!ConnectToDB($database)) //tutaj sama próba połączenia
		throw new Exception('Błąd w trakcie próby połączenia z bazą danych');

	require_once(BASEDIR."includes/view/view.php");
	require_once(BASEDIR."includes/view/elements.php");
	$view = new View();
	
	//ustawienie nagłówka, menu i panelu bocznego
	require_once(BASEDIR."includes/pageElements/header.php");
	require_once(BASEDIR."includes/pageElements/menu.php");
	require_once(BASEDIR."includes/pageElements/sidebar.php");
	$view = SetHeader($view);
	$view = SetMenu($view);
	$view = SetSidebar($view, $database);
	
	if(isset($_GET['action']))
	{
		if($_GET['action'] == 'logout') //wylogowanie
		{
			DestroySession();
			$database -> Disconnect();
			header('location:'.BASEDIR.'index.php');
		}
		else
		{	//sprawdzamy czy żądanie załadowania strony jest poprawne. Jeśli tak - ładujemy ją
			$subpages = array('login', 'register', 'profile', 'deletecategory', 'addcategory', 'editcategory', 'categories', 'category',
				'editprofile', 'deleteprofile', 'trail', 'addtrail', 'deletetrail', 'edittrail', 'trips', 'trip', 'addtrip', 'edittrip');
			if(in_array($_GET['action'], $subpages))
				require_once(BASEDIR."includes/subpages/".$_GET['action'].".php");
			else
				require_once(BASEDIR."includes/subpages/main.php");
		}
	}
	else
		require_once(BASEDIR."includes/subpages/main.php");
	
	$view = SetMain($view, $database); //ustawienie głównej części strony

	//pobieranie szablonu i wyświetlenie strony
	require_once(BASEDIR."includes/view/themeManager.php");
	$theme = ThemeManager :: GetTheme();
	$theme -> Display($view);

	$database -> Disconnect(); //rozłączenie z bazą i zapis sesji
	session_write_close();
}
catch(Exception $ex)
{
	echo 'Wystąpił wewnętrzny błąd serwisu. Niestety, wyświetlenie strony nie jest możliwe';
	if($debug)
		echo '<br/>'.$ex -> getMessage();
}
?>
