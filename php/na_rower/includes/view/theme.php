<?php

/*
Przemysław Pastuszka
2010
*/

require_once(BASEDIR."includes/view/elements.php");
require_once(BASEDIR."includes/view/view.php");

interface Theme
{
	public function Display($view);
}

?>
