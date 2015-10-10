  
<?php

    /*
            Creat: DAN
            Data : 10-octombrie-2015
    */

    include 'helper_functions.php';
     
    function add_inputs() {
        
        $html = "".
        '<div class="formField">'.
            '<div class="formLabel"><label>Site :</label></div>'.
            '<div class="formInput"><input type="text" name="site" id="site" readonly/></div>'.
        '</div>'.
        '<div class="formField">'.
            '<div class="formLabel"><label>Echipa :</label></div>'.
            '<div class="formInput"><input type="text" name="echipa" id="echipa" readonly/></div>'.
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
    
   
    
    echo "<!DOCTYPE html><html><head>". 
            "<link rel='stylesheet' type='text/css' href='css/format_input.css'> ".
            "<script type=\"text/javascript\" src=\"js/jquery-1.11.3.min.js\"></script> ".
            "<script type=\"text/javascript\" src=\"js/call_add_team.js\"></script> ".
            "<script type=\"text/javascript\" src=\"js/gui.js\"></script> ".
            "</head><body>";
    
    echo "<div style='width:100%'>".
         "<div style='float:left;width:60%'>".
            print_untranslated_teams('fotbal').
          "</div>".
          "<div style='float:left;width:40%;'>".
            add_inputs().
          "</div>".
          "</div>";
    
    echo "</body></html>";
   
?>