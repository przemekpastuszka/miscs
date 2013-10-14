<?php

require_once(BASEDIR."includes/view/elements.php");

/*
Przemysław Pastuszka
2010
*/

class TrailData
{
	public $name;
	public $desc;
	public $image;
	public $category;
}

function CreateTrailEditForm(TrailData $data, $action, $showName, $database)
{
	$elements = array();
	if($showName)
		$elements[] = new InputText("name", "Nazwa: *", $data -> name, 50);
		
	//trzeba wypisać wszystkie kategorie
	$categories = array();
	$result = $database -> ExecuteQuery("select nazwa_kat from kategoria order by nazwa_kat");
	if(!$result -> ErrorOccured())
		while($row = $result -> FetchAssoc())
		{
			$cat = new InputOption($row['nazwa_kat'], $row['nazwa_kat']);
			if($row['nazwa_kat'] == $data -> category)
				$cat -> selected = true;
			$categories[] = $cat;
		}
	
	$elements[] = new InputSelect("cat", "Kategoria:", $categories);
	$elements[] = new InputArea("desc", "Opis: ", $data -> desc);
	
	$elements[] = new InputText("points", "", "", 20, "hidden");
	$elements[] = new InputText("MAX_FILE_SIZE", "", "204800", 20, "hidden");
	$elements[] = new InputText("image", "Zdjęcie (max. 200 KB): ", $data -> image, 30, "file");
	
	return new Form("", $action, $elements, 'enctype="multipart/form-data" name ="form" onsubmit="return SetPoints()"');
}
