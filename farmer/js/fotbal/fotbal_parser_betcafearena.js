/*
 * Data : 6-octombrie-2015
 * autor : dan
 */

//http://www.betcafearena.ro/pariuri-sportive/fotbal  - de aici se porneste

var changeBetcafearena_day = function ( day ) {
    
    console.debug(day);
    
    // in divul event days se afla hrefurile cu zilele
    var butoane_zile_main_div = document.querySelectorAll('div.events_days');
    
    // primul div reprezinta grupul de butoane
    var button_group = butoane_zile_main_div[0];
    
    // din div se cauta elementul UL
    var ul_button_group = button_group.childNodes[1];
    
    // din ul se extrag li'urile
    var li_button_group = ul_button_group.childNodes[1];
    
    // se extrage butonul dorit si se apasa pe el
    // din nou mai intai apar mizerii #text in fata deci inmultesc cu 2
    var butoane_zile = li_button_group.childNodes[(day*2)+1].childNodes[0];
    
    setTimeout(function() {
        butoane_zile.click();
    },3000);
    
    

}

var getBetcafearena_Fotbal = function  () {
    
    
    //console.debug("listener fired BET CAFE ARENA");
        
    // pentru bet cafe arena
    // meciurile sunt in tabele ce au clasa odds (ligi)
    
    var query = document.querySelectorAll('table.odds');
        
    //console.debug(query.length);
        
    [].forEach.call( query, 
        function  fn(table){
            // se parseaza fiecare liga in parte
                
            var league_table_nodes = table.childNodes;
            
            //console.debug(league_table_nodes.length);
            
            // folosit pentru a gasit cotele
            // jegosii nu au facut clase separate pentru ficare cota
            // deci tre sa ma iau dupa scrisul din header
            var header_tablel;
            
            
            [].forEach.call(league_table_nodes,
                    function fn(league_table){
                        
                        if (league_table.nodeName.toLowerCase() == "thead") {
                            [].forEach.call(league_table.childNodes,
                                function fn(header_tr) {
                                    if (header_tr.nodeName.toLowerCase() != "tr")
                                        return;
                                    
                                    header_tablel = header_tr.childNodes;
                                }
                            )
                    
                            return;
                        }
                        
                        if (league_table.nodeName.toLowerCase() != "tbody") {
                            return;
                        }
                        
                        // se incepe procesarea efectiva pe tabele
                        // se citesc copii din tbody
                        
                        var tbody_table = league_table.childNodes;
                        
                        
                        [].forEach.call(tbody_table,
                            function (tbody) {
                                
                                // pe tr sunt meciurile
                                if (tbody.nodeName.toLowerCase() != "tr")                             
                                    return;
                                
                                var meci = new meci_fotbal();
                                
                                meci.add_info_betcafearena(header_tablel,tbody.childNodes);
                                if (enable_debug==true)
                                    console.debug(meci.to_text());
                                meci.upload_to_db();
                                
                            }
                        );
                        
                        
                    }
            );
        }
    );
    
    
}