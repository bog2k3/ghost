  
<?php

    /*
            Creat: DAN
            Data : 10-octombrie-2015
    */

    include 'database_setup.php';
    
    //TODO : implementare sporturi 
    // momentan merge doar pe fotbal
    
    function print_untranslated_teams($sport) {
        
        $con = mysqli_connect($servername,$username,$password,$database,$database_port);

        if (!$con) {
            die("Connection failed: " . mysqli_error($con));
        }
        
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
        
        mysqli_free_result($query);
        mysqli_close($con);
        return $html;
    }
    
    function add_inputs() {
        
        $html = "Site : <input type='text' id='site'> <br> Echipa : <input type='text' id='echipa'>";
        
        return $html;
    }
    
    
    
    echo "<div style='width:100%;'>".
         "<div style='float:left;width:60%'>".
            print_untranslated_teams('fotbal').
          "</div>".
          "<div style='float:left;width:40%'>".
            add_inputs().
          "</div>".
          "</div>";
?>