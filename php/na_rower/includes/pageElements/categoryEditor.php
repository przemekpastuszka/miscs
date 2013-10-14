<?php

require_once(BASEDIR."includes/view/elements.php");

/*
Przemysław Pastuszka
2010
*/

class CategoryData
{
	public $name;
	public $desc;
	public $image;
}

function CreateCategoryEditForm(CategoryData $data, $title, $action, $showName)
{
	$elements = array();
	if($showName)
		$elements[] = new InputText("name", "Nazwa: *", $data -> name, 50);
	$elements[] = new InputArea("desc", "Opis: ", $data -> desc);
	
	$elements[] = new InputText("MAX_FILE_SIZE", "", "204800", 20, "hidden");
	$elements[] = new InputText("image", "Zdjęcie (max. 200 KB): ", $data -> image, 30, "file");
	
	return new Form($title, $action, $elements, 'enctype="multipart/form-data"');
}
