<?php

/*
Przemysław Pastuszka
2010
*/

require_once(BASEDIR."templates/pixelgreen/pixelgreen.php");

class ThemeManager
{
	public static function GetTheme()
	{
		return new PixelGreen(); //podobnie jak z bazą danych - tylko jeden template w tym momencie
	}
}

?>
