function add_fotbal_translation() {
    
    var site                = document.getElementById('site').value;
    var team                = document.getElementById('echipa').value;
    var team_translated     = document.getElementById('echipa_tradusa').value;
    
    //console.debug(site);
    //console.debug(team);
    //console.debug(team_translated);
    
    $.ajax( {
           
            type: "GET",
            url: "add_fotbal_translation.php",
            data: "site="+site+"&echipa="+team+"&echipai="+team_translated
    });
    
}