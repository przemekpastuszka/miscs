<?php
/*
Przemysław Pastuszka
2010
*/

define('BASEDIR', "");
define('BASEURL', "");

$debug = true; //należy zmienić na false jeśli strona ma zostać udostępniona publicznie
$author_mail = "autor@domena.pl"; //na ten adres wysyłane będą komunikaty o błędach

if(!isset($debug))
	$debug = false;

//jeśli debugowanie jest włączone to wyświetlamy komunikat
//w przeciwnym wypadku mailujemy do autora strony
function ErrorHandling($e_number, $e_message, $e_file, $e_line, $e_vars)
{
	global $debug;
	global $author_mail;
	
	$message = "Wystąpił błąd w skrypcie $e_file w wierszu $e_line: \n<br/>$e_message\n<br/>";
	$message .= "Data i godzina: ".date('d-m-Y H:i:s')."\n<br/>";
	$message .= "<pre>".print_r($e_vars, 1)."</pre>\n<br/>";

	if($debug)
		echo '<p class="error">'.$message.'</p>';
	else
	{
		error_log($message, 1, $author_mail);

		/*if(($e_number != E_NOTICE) && ($e_number < 2048))
			echo '<p class="error">Wystąpił błąd. Przepraszamy.</p>';*/
	}
}

set_error_handler('ErrorHandling');

?>
