<title>Vota!</title>



<?php

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

function error(){
	echo "errore grave - non puoi votare";
	exit();
}

	$path = "./votes/";

	$voto = $_POST['voto'];

	$ip = get_client_ip();
	$ip_array = explode('.',$ip);
	if (count($ip_array)<>4) error();
	$id=$ip_array[3];
	if ($id=="") error();

?>

<strong>Vota gesu</strong>
<br>
<br>

<? echo "tuo id: ". $id; ?>

<? if ($voto == "" ) { ?>


<form name="form1" method="post" action="#">
<input type="radio" name="voto" value="male">Maschio<br>
<input type="radio" name="voto" value="female">Femena
<input type="submit" value="OK"><br>
</form>


<? } else { 

	//crea file se non esiste gia
	$fp= fopen($path.$id.".vot",'x');
	if ($fp===false){ 
?> 
		hai gia votato!!
<?	} else {

		fwrite($fp,$voto); 
		fclose($fp); 

?>
	        hai votato <? echo $voto;?>

<?
	}

 } ?>
