<?php

/*
	Creat: DAN
	Data : 1-octombrie-2015
*/

include 'database_setup.php';

/**********************************************
	structura tabel
***********************************************

	sesiune 		- uint (10)
	echipa1 		- varchar(32)
	echipa2 		- varchar(32)
	cota_1			- float
	cota_2			- float
	cota_x			- float
	cota_1x			- float
	cota_2x			- float
	cota_12			- float
	site_id_1		- varchar(12)
	game_id_1		- varchar(12)
	timp 			- timestamp (current)
        hash_joc		- varchar(64) (PK)

************************************************/

// se extrag parametrii din post
// folosesc si get si post pentru a testa direct php'ul

$id_sesiune	=	$_POST["sesiune"];
if ($id_sesiune == '') 
$id_sesiune     =       $_GET["sesiune"];
    
$echipa1	=	$_POST["e1"];
if ($echipa1 == '') 
$echipa1	=	$_GET["e1"];

$echipa2	=	$_POST["e2"];
if ($echipa2 == '') 
$echipa2	=	$_GET["e2"];

$cota1		=	$_POST["c1"];
if ($cota1 == '') 
$cota1		=	$_GET["c1"];

$cota2		=	$_POST["c2"];
if ($cota2 == '') 
$cota2		=	$_GET["c2"];

$cotax		=	$_POST["cx"];
if ($cotax == '') 
$cotax		=	$_GET["cx"];

$cota1x		=	$_POST["c1x"];
if ($cota1x == '') 
$cota1x		=	$_GET["c1x"];

$cota2x		=	$_POST["c2x"];
if ($cota2x == '') 
$cota2x		=	$_GET["c2x"];

$cota12		=	$_POST["c12"];
if ($cota12 == '') 
$cota12		=	$_GET["c12"];

$site		=	$_POST["site"];
if ($site == '') 
$site		=	$_GET["site"];

$game		=	$_POST["game_id"];
if ($game == '') 
$game		=	$_GET["game_id"];

// hasul se calculeaza
$hash_game="";


$con = mysqli_connect($servername,$username,$password,$database,$database_port);

if (!$con) {
	die("Connection failed: " . mysqli_error($con));
}


$values = $id_sesiune.','.
        '\''.$echipa1.'\''.','.
        '\''.$echipa2.'\''.','.
        $cota1.','.
        $cota2.','.
        $cotax.','.
        $cota1x.','.
        $cota2x.','.
        $cota12.','.
        '\''.$site.'\''.','.
        '\''.$game.'\'';

$hash_game=hash('sha256',$values);

$values = $values.
          
$query = 'insert into fotbal(sesiune,echipa1,echipa2,cota_1,cota_2,cota_x,cota_1x,cota_2x,cota_12,site_id_1,game_id_1,hash_joc) values ('.
        $values.','.
        '\''.$hash_game.'\''.
        ')';

echo $query;

if (mysqli_query($con,$query)) {
} else {
	die("Error inserting data ".mysqli_error($con));
}



mysqli_close($con);

?>
