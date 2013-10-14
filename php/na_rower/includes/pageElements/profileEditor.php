<?php

require_once(BASEDIR."includes/view/elements.php");

/*
Przemysław Pastuszka
2010
*/

class ProfileData
{
	public $nick;
	public $name;
	public $surname;
	public $desc;
	public $bicycle;	
}

function CreateProfileEditForm(ProfileData $data, $title, $action, $nickAndPassword)
{
	$elements = array();
	if($nickAndPassword)
		$elements[] = new InputText("nick", "Nick: *", $data -> nick, 15);
	$elements[] = new InputText("name", "Nazwisko:", $data -> name, 20);
	$elements[] = new InputText("surname", "Imię:", $data -> surname, 15);
	$elements[] = new InputText("bicycle", "Rodzaj roweru:", $data -> bicycle, 50);
	$elements[] = new InputArea("desc", "Opis:", $data -> desc);
	
	if($nickAndPassword)
	{
		$elements[] = new InputText("password", "Hasło: *", "", 50, "password");
		$elements[] = new InputText("passwordRepeat", "Powtórz hasło: *", "", 50, "password");
	}
	
	return new Form($title, $action, $elements);
}

?>
