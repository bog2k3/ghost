  
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
        
        $html = '<table >';
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
        
        mysqli_free_result($result);
        
        return $html;
    }
    
    function add_inputs() {
        
        $html = "".
        '<div class="formField">'.
            '<div class="formLabel"><label>Site :</label></div>'.
            '<div class="formInput"><input type="text" name="site" id="site" /></div>'.
        '</div>'.
        '<div class="formField">'.
            '<div class="formLabel"><label>Echipa :</label></div>'.
            '<div class="formInput"><input type="text" name="echipa" id="echipa" /></div>'.
        '</div>'.
        '<div class="formField">'.
            '<div class="formLabel"><label>Traducere :</label></div>'.
            '<div class="formInput"><input type="text" name="echipa" id="echipa_tradusa" /></div>'.
        '</div>'.
        '<br/>'.
        '<button type="button" onclick=add_fotbal_translation()>OK</button>'
        ;
        
        
        return $html;
    }
    
    $con = mysqli_connect($servername,$username,$password,$database,$database_port);

    if (!$con) {
            die("Connection failed: " . mysqli_error($con));
    }
    
    echo "<!DOCTYPE html><html><head>". 
            "<link rel='stylesheet' type='text/css' href='css/format_input.css'> ".
            "<script type=\"text/javascript\" src=\"js/jquery-1.11.3.min.js\"></script> ".
            "<script type=\"text/javascript\" src=\"js/call_add_team.js\"></script> ".
            "</head><body>";
    
    echo "<div style='width:100%;'>".
         "<div style='float:left;width:60%'>".
            print_untranslated_teams($con,'fotbal').
          "</div>".
          "<div style='float:left;width:40%'>".
            add_inputs().
          "</div>".
          "</div>";
    
    echo "</body></html>";
    mysqli_close($con);
?>