<?php

    /*
            Creat: DAN
            Data : 10-octombrie-2015
    */
    
    include 'database_setup.php';
    
    /**********************************************
            structura tabel
    ***********************************************
     
        site                - varchar(12)
        echipa              - varchar(32)
        echipa_internal     - varchar(32)
     
    ************************************************/

    // se extrag parametrii din post
    // folosesc si get si post pentru a testa direct php'ul
    
    $site                   =       $_POST["site"];
    if ($site == '') {
        $site               =       $_GET["site"];
    }
    
    $echipa                 =       $_POST["echipa"];
    if ($echipa == '') {
        $echipa             =       $_GET["echipa"];
    }
    
    $echipa_internal        =       $_POST["echipai"];
    if ($echipa_internal == '') {
        $echipa_internal    =       $_GET["echipai"];
    }
    
    $con = mysqli_connect($servername,$username,$password,$database,$database_port);

    if (!$con) {
            die("Connection failed: " . mysqli_error($con));
    }
    
    $query = "insert into fotbal_dict(site,echipa,echipa_internal) values (".
             "'".$site."',".
            "'".$echipa."',".
            "'".$echipa_internal."')"
        ;
    
    if (mysqli_query($con,$query)) {

    } else {
        echo $query;
        die("Error inserting data ".mysqli_error($con));
    }

    mysqli_close($con);
    
?>