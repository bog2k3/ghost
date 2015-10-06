//http://www.betcafearena.ro/pariuri-sportive/fotbal  - de aici se porneste

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
                                console.debug(meci.to_text());
                            }
                        );
                        
                        
                    }
            );
        }
    );
    
    
}