<?php


    /*
        Creat: DAN
        Data : 6-octombrie-2015
    * 
    */

    include 'database_setup.php';
    
    $con = mysqli_connect($servername,$username,$password,$database,$database_port);

    if (!$con) {
            die("Connection failed: " . mysqli_error($con));
    }
    
    // pastram o zi in plus datele
    // format data zz/ll/aaaa
    $query = 'delete from fotbal where STR_TO_DATE(data_joc,\'%d/%m/%Y\') < CURDATE()';
    if (mysqli_query($con,$query)) {
        
    } else {
        echo $query;
        die("Error deleting data ".mysqli_error($con));
    }
    
    mysqli_free_result($query);
    
    echo $date;
?>