  
<?php

    /*
            Creat: DAN
            Data : 10-octombrie-2015
    */

    include 'database_setup.php';
    
    //TODO : implementare sporturi 
    // momentan merge doar pe fotbal
    
    function print_untranslated_teams($con,$sport) {
        
        $query = "select * from ( ".
                 " select echipa1 as echipa,site_id_1 as site from fotbal where status_echipe in (1,3) "
                 ." union ".
                 " select echipa2 as echipa,site_id_1 as site from fotbal where status_echipe in (2,3) ".
                " ) as t order by t.site";
        
        $result = mysqli_query($con,$query);
        
        //echo $query;
        
        $html = '<table>';
        $html .= '<tr><th> Site </th><th> Echipa 2 </th>';
        
        while ($row = mysqli_fetch_assoc($result)) {
            $html .= '<tr>';
            
            $html .= '<td><b>';
            $html .= $row["site"];
            $html .= '</b></td>';
            
            $html .= '<td>';
            $html .= $row["echipa"];
            $html .= '</td>';
            
            $html .= '</tr>';
            
            
        }
        
        $html .= '</table>';
        
        echo $html;
        mysqli_free_result($query);
        
    }
    
    $con = mysqli_connect($servername,$username,$password,$database,$database_port);

    if (!$con) {
            die("Connection failed: " . mysqli_error($con));
    }
    
    print_untranslated_teams($con,'fotbal');
?>