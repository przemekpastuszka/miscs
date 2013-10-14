<?php
/*
Przemysław Pastuszka
2010

Znajdują się tutaj klasy, które będą wykorzystane do przechowania struktury strony
*/

class Header
{
	public $language;
	public $keywords;
	public $description;
	public $author;
	public $robots;
	public $charset;
	public $title;

	public $other;
}

interface Drawable
{
	public function GetType();
}

class SidebarElement implements Drawable
{
	public $title;
	public $content;
	
	public function __construct($title, $content)
	{
		$this -> title = $title;
		$this -> content = $content;
	}
	
	public function GetType() {return "SideBarElement";}
}

class Link implements Drawable
{
	public $text;
	public $link;
	public $alt;
	public $highlighted;
	public $shifted;
	
	
	public function __construct($text, $link, $shifted = false)
	{
		$this -> text = $text;
		$this -> link = $link;
		$this -> highlighted = false;
		$this -> shifted = $shifted;
	}
	
	public function GetType() {return "Link";}
}

class MainMenu implements Drawable
{
	public $linksArray;
	
	public function GetType() {return "MainMenu";}
}


class SideMenu implements Drawable
{
	public $linksArray;
	
	public function __construct($links) {$this -> linksArray = $links;}
	
	public function GetType() {return "SideMenu";}
}

class Text implements Drawable
{
	public $text;
	
	public function __construct($text) {$this -> text = $text;}
	
	public function GetType() {return "Text";}
}

class Title implements Drawable
{
	public $text;
	
	public function __construct($text) {$this -> text = $text;}
	
	public function GetType() {return "Title";}
}

class Post implements Drawable
{
	public $title;
	public $author;
	public $readmore;
	public $content;
	public $date;
	public $other;

	public function __construct($title, $author, $content, $date, $readmore, $other)
	{
		$this -> title = $title;
		$this -> author = $author;
		$this -> content = $content;
		$this -> date = $date;
		$this -> readmore = $readmore;
		$this -> other = $other;
	}
	
	public function GetType() {return "Post";}
}

class Form implements Drawable
{
	public $title;
	public $action;
	public $elements;
	public $other;
	
	public function __construct($title, $action, $elements, $other = "")
	{
		$this -> title = $title;
		$this -> action = $action;
		$this -> elements = $elements;
		$this -> other = $other;
	}	
	
	public function GetType() {return "Form";}
}

class InputText implements Drawable
{
	public $name;
	public $value;
	public $type;
	public $size;
	public $label;
	
	public function __construct($name, $label, $value = "", $size = "30", $type = "text")
	{
		$this -> name = $name;
		$this -> type = $type;
		$this -> size = $size;
		$this -> value = $value;
		$this -> label = $label;
	}
	
	public function GetType() {return "InputText";}
}

class InputArea implements Drawable
{
	public $name;
	public $rows;
	public $columns;
	public $value;
	public $label;
	
	public function __construct($name, $label, $value = "",  $rows = "10", $cols = "50")
	{
		$this -> name = $name;
		$this -> value = $value;
		$this -> rows = $rows;
		$this -> columns = $cols;
		$this -> label = $label;
	}
	
	public function GetType() {return "InputArea";}
}
class InputOption implements Drawable
{
	public $name;
	public $value;
	public $selected;
	public $disabled;
	public function __construct($name, $value, $selected = false, $disabled = false)
	{
		$this -> name = $name;
		$this -> value = $value;
		$this -> selected = $selected;
		$this -> disabled = $disabled;
	}
	public function GetType() {return "InputOption";}
}
class InputSelect implements Drawable
{
	public $name;
	public $label;
	public $options;
	
	public function __construct($name, $label, $options)
	{
		$this -> name = $name;
		$this -> options = $options;
		$this -> label = $label;
	}
	public function GetType() {return "InputSelect";}
}
class Table implements Drawable
{
	public $rows;
	public $header;
	
	public function __construct($rows, $header = array())
	{
		$this -> rows = $rows;
		$this -> header = $header;
	}
	
	public function GetType() {return "Table";}
}
class Image implements Drawable
{
	public $img;
	public $width;
	public $height;
	public $align;
	
	public function __construct($img, $width, $height, $align = "left")
	{
		$this -> img = $img;
		$this -> width = $width;
		$this -> height = $height;
		$this -> align = $align;
	}
	
	public function GetType() {return "Image";}
}

?>
