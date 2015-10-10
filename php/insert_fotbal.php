<?php

    /*
            Creat: DAN
            Data : 1-octombrie-2015
            Data : 6-octombrie-2015 : Verificare existenta hash pentru insert/update
            Data : 10-octombrie-2015 : mutat functia de verificare existenta traducere
    */

    include 'database_setup.php';
    include 'helper_functions.php';

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
            hash_joc_comun          - varchar(64)
            echipa1_internal        - varchar(32)
            echipa2_internal        - varchar(32)
            data_joc                - varchar(10)
            status_echipe           - int(1) - valori posibile 0 - echipele exista in DD 1 - echipa1 nu exista 2-echipa2 nu exista 3-niciuna din echipe nu exista
    ************************************************/

    
    
    /**
     * echipa1_internal si echipa2_internal sunt echipele traduse 
     * folosind dictionarul 
     */

    // se extrag parametrii din post
    // folosesc si get si post pentru a testa direct php'ul

    $id_sesiune         =	$_POST["sesiune"];
    if ($id_sesiune == '') {
        $id_sesiune     =       $_GET["sesiune"];
    }

    $echipa1            =	$_POST["e1"];
    if ($echipa1 == '') {
        $echipa1        =	$_GET["e1"];
    }

    $echipa2            =	$_POST["e2"];
    if ($echipa2 == '') {
        $echipa2        =	$_GET["e2"];
    }

    $cota1		=	$_POST["c1"];
    if ($cota1 == '') {
        $cota1		=	$_GET["c1"];
    }
    
    $cota2		=	$_POST["c2"];
    if ($cota2 == '') {
        $cota2		=	$_GET["c2"];
    }
    
    $cotax		=	$_POST["cx"];
    if ($cotax == '') {
        $cotax		=	$_GET["cx"];
    }
    
    $cota1x		=	$_POST["c1x"];
    if ($cota1x == '') 
    $cota1x		=	$_GET["c1x"];

    $cota2x		=	$_POST["c2x"];
    if ($cota2x == '') {
        $cota2x		=	$_GET["c2x"];
    }
    
    $cota12		=	$_POST["c12"];
    if ($cota12 == '') {
        $cota12		=	$_GET["c12"];
    }
    
    $site		=	$_POST["site"];
    if ($site == '') {
        $site		=	$_GET["site"];
    }
    
    $game		=	$_POST["game_id"];
    if ($game == '') {
        $game		=	$_GET["game_id"];
    }
    
    $game_date          =       $_POST["game_date"];
    if ($game_date == '') {
        $game_date          =	$_GET["game_date"];
    }
    
    // TODO : trebuie facut dictionarul de echipe/site
    // TODO : tabela de erori

   

    $con = mysqli_connect($servername,$username,$password,$database,$database_port);

    if (!$con) {
            die("Connection failed: " . mysqli_error($con));
    }
    
    $status_echipe=0;
    
    // se extrag echipele traduse din baza de date
    $echipa1_internal = getInternalTeam($con,$echipa1,$site); 
    if ($echipa1_internal == ''){
        $echipa1_internal=$echipa1;
        $status_echipe+=1;
    }
    
    $echipa2_internal = getInternalTeam($con,$echipa2,$site); 
    if ($echipa2_internal == ''){
        $echipa2_internal=$echipa2;
        $status_echipe+=2;
    }
    
    // hash cheie primara gid1.gid2.site.gdate
    $hash_game_pk=hash('sha256',$echipa1_internal.$echipa2_internal.$site.$game_date);

    // hash joc comun
    $hash_game_comun=hash('sha256',$echipa1_internal.$echipa2_internal.$site);

    //verifica PK
    $query = 'select hash_joc from fotbal where hash_joc=\''.$hash_game_pk.'\'';
    $result = mysqli_query($con,$query);


    if (mysqli_num_rows($result) > 0) {

        // se creaza updateul
        $query = 'update fotbal set '.
                'sesiune = '.$id_sesiune.','.
                'cota_1 = '.$cota1.','.
                'cota_2 = '.$cota2.','.
                'cota_x = '.$cotax.','.
                'cota_1x = '.$cota1x.','.
                'cota_2x = '.$cota2x.','.
                'cota_12 = '.$cota12.','.
                'game_id_1 = '.'\''.$game.'\''.
                ' where hash_joc='.'\''.$hash_game_pk.'\'';

    } else {

        // se creaza insertup

        $query = 'insert into fotbal(sesiune,echipa1,echipa2,cota_1,cota_2,cota_x,cota_1x,cota_2x,cota_12,site_id_1,game_id_1,hash_joc,hash_joc_comun,echipa1_internal,echipa2_internal,data_joc,status_echipe) values ('.
                $id_sesiune.','.
                '\''.$echipa1.'\''.','.
                '\''.$echipa2.'\''.','.
                $cota1.','.
                $cota2.','.
                $cotax.','.
                $cota1x.','.
                $cota2x.','.
                $cota12.','.
                '\''.$site.'\''.','.
                '\''.$game.'\''.','.
                '\''.$hash_game_pk.'\','.
                '\''.$hash_game_comun.'\','.
                '\''.$echipa1_internal.'\','.
                '\''.$echipa2_internal.'\','.
                '\''.$game_date.'\','.
                $status_echipe.
                ')';
    }


    //echo $query;

    if (($result = mysqli_query($con,$query))) {

    } else {
        echo $query;
        die("Error inserting data ".mysqli_error($con));
    }
    
    mysqli_free_result($result);
    mysqli_close($con);

?>
