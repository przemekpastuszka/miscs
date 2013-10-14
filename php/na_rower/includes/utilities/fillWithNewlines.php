<?php

/*
Przemysław Pastuszka
2010
*/


//pojawiał się problem podczas wyświetlania obrazków - tj. całość nieco się rozjeżdżała
//rozwiązanie - uzupełniać tekst towarzyszący obrazkowi o znaczniki nowej linii
function FillWithNewlines($string, $n)
{
	$m = substr_count($string, "<br />");
	if($m < $n)
		for($i = 0; $i < $n - $m; ++$i)
			$string .= "<br />";
	return $string;
}

?>
