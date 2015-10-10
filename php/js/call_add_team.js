function add_fotbal_translation() {
    
    var site                = document.getElementById('site').value;
    var team                = document.getElementById('echipa').value;
    var team_translated     = document.getElementById('echipa_tradusa').value;
    
    $.ajax( {
           
            type: "GET",
            url: "add_fotbal_translation.php",
            data: "site="+site+"&echipa="+team+"&echipai="+team_translated,
            success : function(msg) {
                
                //se refreshuieste tabelul
                
                $.ajax( {
                    
                    type: "GET",
                    url: "refresh_translation_table_fotblal.php",
                    success : function (msg) {
                        
                        $('#ftable').html(msg);
                        
                    }
                });
            }
            
    });
    
}

function set_intput_values(site,team,team_translated) {
    document.getElementById('site').value = site;
    document.getElementById('echipa').value = team;
    document.getElementById('echipa_tradusa').value = team_translated;
}