<?php

 /*
            Creat: DAN
            Data : 10-octombrie-2015
            Data : 3-decembrie-2015 : Se foloseste traducatorul nu baza de date.
  */

    function    getInternalTeam($connection,$team) {

        include 'database_setup.php';
        
        
        $TRAD_COMMAND = $trad_path." --cpath ".$trad_cache." --sport fotbal --lpath ".$trad_liste." \"".$team."\"";//." 2>&1";
        
        $RETURN_VALUE = shell_exec($TRAD_COMMAND);
        
        if (is_null($RETURN_VALUE)) {
            $RETURN_VALUE="ERROR";
        }
        
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