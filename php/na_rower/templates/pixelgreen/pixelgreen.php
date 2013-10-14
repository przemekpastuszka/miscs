<?php

require_once(BASEDIR."includes/view/theme.php");

class PixelGreen implements Theme
{	
	public function Display($view)
	{
		//echo '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">';
		if(isset($view -> header -> language))
			echo '<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="'.$view -> header -> language.'" lang="'.$view -> header -> language.'">';
		$this -> DisplayHeader($view -> header, "templates/pixelgreen/images/PixelGreen.css", "");
		$this -> DisplayBody($view);
		echo '</html>';
	}
	
	private function DisplayHeader($header, $stylesheet, $additionalStuff)
	{
		echo '<head>';
		if(isset($header -> description))
			echo '<meta name="Description" content="'.$header -> description.'" />';
		if(isset($header -> keywords))
			echo '<meta name="Keywords" content="'.$header -> keywords.'" />';
		if(isset($header -> author))
			echo '<meta name="Author" content="'.$header -> author.'" />';
		if(isset($header -> robots))
			echo '<meta name="Robots" content="'.$header -> robots.'" />';
		if(isset($header -> charset))
			echo '<meta http-equiv="Content-Type" content="text/html; charset='.$header -> charset.'" />';
			
		echo '<link rel="stylesheet" href="'.$stylesheet.'" type="text/css" />';
		
		if(isset($header -> title))
			echo '<title>'.$header -> title.'</title>';
		
		if(isset($header -> other))
			echo $header -> other;
		
		echo $additionalStuff;	
			
		echo '</head>';
	}
	
	private function DisplayBody($view)
	{
		if(isset($view -> onloadAction))
			echo '<body onload="'.$view -> onloadAction.'">';
		else
			echo '<body>';
		echo '<div id="wrap">';
		echo '<div id="header"><div id="header-content">';	
		echo '<h1 id="logo"><a href="index.php" title="">na<span class="gray">rower</span></a></h1>';
		
		if(isset($view -> slogan))
			echo '<h2 id="slogan">'.$view -> slogan.'</h2>';
			
		if(isset($view -> menu))
			$this -> DisplayMenu($view -> menu, "");
				
		echo '</div></div>';
		echo '<div class="headerphoto"></div>';
		echo '<div id="content-wrap"><div id="content">';
		
		if(isset($view -> sidebar))
			$this -> DisplaySideBar($view);
		
		if(isset($view -> main))
			$this -> DisplayMain($view);
		
		echo '</div></div>';
		
		$this -> DisplayFooter();
		
		echo '</div>';
		echo '</body>';
	}
	
	protected function DisplaySideBar($view)
	{
		echo '<div id = "sidebar">';
		
		foreach($view -> sidebar as $sidebarElement)
		{
			echo '<div class = "sidebox">';
			echo '<h1>'.$sidebarElement -> title.'</h1>';
			
			if(isset($sidebarElement -> content))
				$this -> DisplayElement($sidebarElement -> content);
			echo '</div>';
		}
		
		echo '</div>';
	}
	
	protected function DisplayMain($view)
	{
		echo '<div id = "main">';
		
		foreach($view -> main as $mainElement)
			$this -> DisplayElement($mainElement);
		
		echo '</div>';
	}
	
	protected function DisplayLink(Link $link, $class)
	{
		if($link -> shifted)
			echo '<p>';
		echo '<a href = "'.$link -> link.'" ';
		if($link -> highlighted)
			echo 'id = "current" ';
		if(isset($link -> alt))
			echo 'alt = "'.$link -> alt.'" ';
		if(isset($class) && $class != "")
			echo 'class = "'.$class.'"';
		echo '>';
		echo $link -> text;
		echo '</a>';
		if($link -> shifted)
			echo '</p>';
	}
	
	protected function DisplayMenu($menu, $class)
	{
		if(isset($class) && $class != "")
			echo '<ul class = "'.$class.'">';
		else
			echo '<ul>';
			
		foreach($menu as $link)
		{
			echo '<li>';
			$this -> DisplayLink($link, "");
			echo '</li>';
		}
		
		echo '</ul>';
	}
	
	protected function DisplayElement(Drawable $element)
	{
		switch($element -> GetType())
		{
			case "Text":
				$this -> DisplayText($element);
				break;
				
			case "Title":
				$this -> DisplayTitle($element);
				break;
				
			case "Link":
				$this -> DisplayLink($element, "");
				break;
				
			case "Post":
				$this -> DisplayPost($element);
				break;
				
			case "SideMenu":
				$this -> DisplayMenu($element -> linksArray, "sidemenu");
				break;
				
			case "Form":
				$this -> DisplayForm($element);
				break;
			
			case "InputText":
				$this -> DisplayInputText($element);
				break;
				
			case "InputArea":
				$this -> DisplayInputArea($element);
				break;
				
			case "InputSelect":
				$this -> DisplayInputSelect($element);
				break;	
				
			case "Table":
				$this -> DisplayTable($element);
				break;
				
			case "Image":
				$this -> DisplayImage($element);
				break;
		}
	}
	
	protected function DisplayText(Text $text)
	{
		echo '<p>'.$text -> text.'</p>';
	}
	
	protected function DisplayTitle(Title $text)
	{
		echo '<h3>'.$text -> text.'</h3>';
	}
	
	protected function DisplayPost(Post $post)
	{
		echo '<div class = "post">';
		echo '<a name = "'.$post -> title.'"></a>';
		echo '<h1>'.$post -> title.'</h1>';
		echo '<p>Autor: ';
		$this -> DisplayElement($post -> author);
		echo '</p>';
		
		foreach($post -> content as $element)
			$this -> DisplayElement($element);
			
		echo '<p class="post-footer align-right">';
		
		if(isset($post -> readmore) && $post -> readmore != "")
			$this -> DisplayLink($post -> readmore, "readmore");
		if(isset($post -> date) && $post -> date != "")
			echo '<span class="date">'.$post -> date.'</span>';
			
		foreach($post -> other as $element)
			$this -> DisplayElement($element);
		echo '</p>';
		echo '</div>';
	}
	
	protected function DisplayImage(Image $i)
	{
		printf("<p><img src='%s' ", $i -> img);
		if(isset($i -> width) && $i -> width != 0)
			printf("width = '%s' ", $i -> width);
		if(isset($i -> height) && $i -> height != 0)
			printf("height = '%s' ", $i -> height);
		printf("class='float-%s' /></p>", $i -> align);
	}
	
	protected function DisplayForm(Form $form)
	{
		if(isset($form -> title) && $form -> title != "")
			echo '<h3>'.$form -> title.'</h3>';
		echo '<form method=post action="'.$form -> action.'" '.$form -> other.'>';
		echo '<p>';
		
		foreach($form -> elements as $element)
		{
			if($element -> label != "")
				echo '<label>'.$element -> label.'</label>';
			$this -> DisplayElement($element);
		}
		
		echo '<br /><br />';
		echo '<input class="button" type="submit" />';
		echo '</p>';
		echo '</form>';
	}
	
	protected function DisplayInputText(InputText $input)
	{
		echo '<input name="'.$input -> name.'" value="'.$input -> value.'" type="'.$input -> type.'" size="'.$input -> size.'" />';
	}
	
	protected function DisplayInputArea(InputArea $input)
	{
		echo '<textarea name ="'.$input -> name.'" rows="'.$input -> rows.'" cols="'.$input -> columns.'">';
		echo $input -> value;
		echo '</textarea>';
	}
	
	protected function DisplayInputSelect(InputSelect $input)
	{
		echo '<select name="'.$input -> name.'">';
		foreach($input -> options as $option)
		{
			echo '<option value="'.$option -> name.'" ';
			if($option -> selected)
				echo 'selected ';
			if($option -> disabled)
				echo 'disabled ';
			echo '>'.$option -> value.'</option>';
		}
		echo '</select>';
	}
	
	protected function DisplayTable(Table $table)
	{
		echo '<table>';
		
		if(isset($table -> header))
		{
			echo '<tr>';
			foreach($table -> header as $elem)
			{
				echo '<th>';
				$this -> DisplayElement($elem);
				echo '</th>';
			}
			echo '</tr>';
		}
		
		$i = 0;
		foreach($table -> rows as $row)
		{
			echo '<tr class="row-';
			if($i % 2 == 0)
				echo 'a';
			else
				echo 'b';
			echo '">';
			
			foreach($row as $elem)
			{
				echo '<td>';
				$this -> DisplayElement($elem);
				echo '</td>';
			}
			
			echo '</tr>';
			++$i;
		}
		echo '</table>';
	}
	
	protected function DisplayFooter()
	{
		echo '<div id="footer"><div id="footer-content">	
		<div class="col2 float-right">

		<p>
		&copy; copyright 2010 <strong>Przemysław Pastuszka</strong><br /> 
		Design by: <a href="index.html"><strong>styleshout</strong></a> &nbsp; &nbsp;
		Valid <a href="http://jigsaw.w3.org/css-validator/check/referer"><strong>CSS</strong></a> | 
		      <a href="http://validator.w3.org/check/referer"><strong>XHTML</strong></a><br />

		Image by: <a href="http://www.freedigitalphotos.net/images/view_photog.php?photogid=178">Tom Curtis / FreeDigitalPhotos.net</a>
		</p>
		</div>
	
		</div></div>';
	}
}

?>
