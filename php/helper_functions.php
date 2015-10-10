<?php

 /*
            Creat: DAN
            Data : 10-octombrie-2015
  */

    function    getInternalTeam($connection,$team,$site) {
                
        $query = 'select echipa_internal from fotbal_dict where'.
                 'site = \''.$site.'\' and echipa = \''.$team.'\'';
        
        $result = mysqli_query($connection,$query);
        
        $RETURN_VALUE;

        if (mysqli_num_rows($result) > 0) {
            
             if (($row = mysqli_fetch_assoc($result))) {
                 
                 $RETURN_VALUE =  $row["echipa_internal"];
             }
        }        
        mysqli_free_result($query);
        return $RETURN_VALUE;
    }
    
    function print_untranslated_teams($sport) {
        
        include 'database_setup.php';
        
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
        
        $html = '<table id="ftable">';
        $html .= '<tr><th> Site </th><th> Echipa 2 </th>';
        
        while ($row = mysqli_fetch_assoc($result)) {
            
            if (getInternalTeam($con,$row["echipa"],$row["site"]) == '') {
                $html .= '<tr onClick="HighLightTR(this,\'#c9cc99\',\'cc3333\');set_intput_values(\''.$row["site"].'\',\''.$row["echipa"].'\',\'\');">';

                $html .= '<td><b>';
                $html .= $row["site"];
                $html .= '</b></td>';

                $html .= '<td>';
                $html .= $row["echipa"];
                $html .= '</td>';

                $html .= '</tr>';
            }
            
        }
        
        $html .= '</table>';
        
        mysqli_free_result($result);
        
        return $html;
        
        mysqli_close($con);
    }
    
?>