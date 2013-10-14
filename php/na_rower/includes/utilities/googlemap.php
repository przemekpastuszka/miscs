<?php

/*
Przemysław Pastuszka
2010

Skrypt do obsługi mapy
*/

require_once(BASEDIR."includes/view/view.php");
require_once(BASEDIR."includes/view/elements.php");

class Point
{
	public $x;
	public $y;
	
	public function __construct($x, $y)
	{
		$this -> x = $x;
		$this -> y = $y;
	}
}

function AddGoogleMap($view, $points, $changeable)
{
	$script = "
		<script type='text/javascript' src='http://maps.google.com/maps/api/js?sensor=false'></script>
		<script type='text/javascript'>
		var map;
		var markers = [];
		var points = [];
		var polyline;
		function initialize() 
		{
			//mapa początkowo pokazuje Polskę
			var latlng = new google.maps.LatLng(52.066,19.379883);
			var myOptions = 
			{
				zoom: 6,
				center: latlng,
				mapTypeId: google.maps.MapTypeId.ROADMAP
			};
			
			map = new google.maps.Map(document.getElementById('map_canvas'), myOptions);
			SetInitMarkers();";
			
	if($changeable) //czy pozwalamy użytkownikowi na dodawanie punktów na mapie
	{
		$script .= "google.maps.event.addListener(map, 'click', function(event)
			{	//po kliknięciu ustaw marker
				PlaceMarker(event.latLng);
			});";
	}

	$script .= "}	//funkcja rysowania trasy
		function DrawLine()
		{
			if(polyline != null)
				polyline.setMap(null); //wymaż starą trasę
			polyline = null;
			points = [];
			var l = markers.length;
			for(var i = 0; i < l; ++i)
			{	//pobierz pozycje markerów
				points.push(markers[i].getPosition());
			}
			polyline = new google.maps.Polyline(
			{
				path: points,
				strokeColor: '#FF0000',
				strokeOpacity: 0.6,
				strokeWeight: 4
			});

			polyline.setMap(map); //narysuj mapę

		}
		function PlaceMarker(location)
		{
			var marker = new google.maps.Marker(
			{
				position : location,
				map : map
			});
			
			//co robimy w razie kliknięcia na marker
			google.maps.event.addListener(marker, 'click', function() {
				marker.setMap(null); //wymazujemy go z mapy
				markers.splice(markers.indexOf(marker), 1); //usuwamy z tablicy markerów
				DrawLine(); //rysujemy trasę raz jeszcze
			});
			markers.push(marker);
			DrawLine();
		}
		function SetPoints() //ta funkcja wypełnia ukryte pole formularza punktami
		{
			realPoints = [];
			var l = markers.length;
			for(var i = 0; i < l; ++i)
			{
				pos = markers[i].getPosition();
				realPoints.push(pos.lat());
				realPoints.push(pos.lng());
			}
			document.form.points.value = realPoints;
			return true;
		}
		";
		$script .= "function SetInitMarkers() {";
		if(count($points) != 0) //dodanie punktów wczytanych z bazy
		{
			foreach($points as $p)
				$script .= sprintf("PlaceMarker(new google.maps.LatLng(%s, %s));", $p -> x, $p -> y);
			if(count($points) > 1) //znalezienie najbardziej wysuniętych punktów, by wypozycjonować widok mapy
			{
				$SW = new Point(1000, 1000);
				$NE = new Point(-1000, -1000);
				foreach($points as $p)
				{
					if($p -> x < $SW -> x)
						$SW -> x = $p -> x;
					if($p -> y < $SW -> y)
						$SW -> y = $p -> y;
					if($p -> x > $NE -> x)
						$NE -> x = $p -> x;
					if($p -> y > $NE -> y)
						$NE -> y = $p -> y;
				}
				$script .= sprintf("var sw = new google.maps.LatLng(%s, %s);
				var ne = new google.maps.LatLng(%s, %s);
				var bounds = new google.maps.LatLngBounds(sw, ne);
				map.fitBounds(bounds);", $SW -> x, $SW -> y, $NE -> x, $NE -> y);
			}
			
		}
		$script .= "DrawLine();}";
		$script .= "</script>";
	$view -> header -> other = $script;
	$view -> onloadAction = "initialize()";
	$view -> main[] = new Text('<div id="map_canvas" style="width:520; height:440"></div>');
	return $view;
}

?>
