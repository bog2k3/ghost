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
        } else {
            $RETURN_VALUE = substr($RETURN_VALUE,0,-1);
        }
        
        return $RETURN_VALUE;
    }
    
    function send_mail($message) {
        
        mail('','Maimuta PHP',$message);
        
    }
    
    
?>