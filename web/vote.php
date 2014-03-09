<?php
//This file is part of Quickpoll.

//Author: Mauro Alessandro (www.maetech.it) (c) march 2014

//Quickpoll is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//Foobar is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with Foobar.  If not, see <http://www.gnu.org/licenses/>.


function get_client_ip() {
     $ipaddress = '';
     if (getenv('HTTP_CLIENT_IP'))
         $ipaddress = getenv('HTTP_CLIENT_IP');
     else if(getenv('HTTP_X_FORWARDED_FOR'))
         $ipaddress = getenv('HTTP_X_FORWARDED_FOR');
     else if(getenv('HTTP_X_FORWARDED'))
         $ipaddress = getenv('HTTP_X_FORWARDED');
     else if(getenv('HTTP_FORWARDED_FOR'))
         $ipaddress = getenv('HTTP_FORWARDED_FOR');
     else if(getenv('HTTP_FORWARDED'))
        $ipaddress = getenv('HTTP_FORWARDED');
     else if(getenv('REMOTE_ADDR'))
         $ipaddress = getenv('REMOTE_ADDR');
     else
         $ipaddress = 'UNKNOWN';

     return $ipaddress; 
}

?>

<!DOCTYPE html>
<html>

<head>
<meta name="viewport" content="width=device-width; initial-scale=1.0; maximum-scale=1.0; user-scalable=0;" />

<title>VotaPhone!</title>
<link href="../style.css" rel="stylesheet" type="text/css" />

</head>


<body>
<center>

<div class="code">

<?php

function error(){
	?> <div id="error">errore grave <br/> non puoi votare</div> <?php
	exit();
}

function giavotato(){
	?> <div class="already">Hai gi&agrave; votato per %pollname%</div> <?php
}

function form(){
?>
<h2>Stai votando per:</h2>
<h1>%pollname%</h1>

<br />

<form name="form1" method="post" action="#">
<input type="radio" id="p1" name="voto" value="1" /><label for="p1"><img src="images/p1.png" /></label>
<input type="radio" id="p3" name="voto" value="3" /><label for="p3"><img src="images/p3.png" /></label>
<input type="radio" id="p5" name="voto" value="5" /><label for="p5"><img src="images/p5.png" /></label>
<br/><br />
<input type="submit" id="ok" value="OK"><label for="ok"><img src="images/ok.png" /></label><br />
</form>
<?php
}

	$path = "./votes/";

	if (isset($_POST['voto'])) { $voto = $_POST['voto']; }
	else { $voto=""; }

	$ip = get_client_ip();
	$ip_array = explode('.',$ip);
	if (count($ip_array)<>4) error();
	$id=$ip_array[3];
	if ($id=="") error();
	$file=$path.$id.".vot";

?>

</div>

<img class="logo" src="images/logo.png" width="100%" />
<br />

<?php
if (file_exists($file)){
	giavotato();
} else {
	if ($voto == "" ) {
		form();
	} else {
		?><div class="code"><?php
		$fp=fopen($file,'x');
		if ($fp===false) {
			error();
		} else {
			fwrite($fp,$voto); 
			fclose($fp); 
			?> </div><div class="voted">hai votato <?php echo $voto; ?> per %pollname%<?php
		}
		?></div><?php
	}
} ?>

<br/>
<div id="reload"><a href="index.php">ricarica</a></div>
<div id="about">QuickPoll by MaeTech</a> for ilMich<br/>www.maetech.it</div>
</center>
</body>
</html>
