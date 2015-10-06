var getStanley_Fotbal = function  () {        
        
        //console.debug("listener fired.");
        
        // pentru stanley bet
        
        var query = document.querySelectorAll('table.pariu');
        
        console.debug(query.length);
        
        
        [].forEach.call( query, 
		function  fn(table){
                    
                    //
                    // practic fiecare meci e un tabel 
                    // macar e mai usor pentru mine sa le gasesc pe toate
                    // 
                     
                    
                    //console.debug(table);
                    
                    var table_nodes = table.childNodes;
                    
                    // se cauta copilul tbody
                    
                    for (var i=0,n=table_nodes.length;i<n;i++) {
                        if (table_nodes[i].nodeName.toLowerCase() != "tbody") continue;
                        //console.debug(table_nodes[i].nodeName);
                        
                        // se citesc copii din tbody
                        var tbody = table_nodes[i].childNodes;
                        var meci_fotbal_rec = new meci_fotbal();
                        
                        for (var j=0,n_tbody=tbody.length;j<n_tbody;j++) {
                            if (tbody[i].nodeName.toLowerCase() == "tr") {
                                // pe tr se gasesc toate informatiile despre meci in copii de tip td
                                
                                var informatii_meci = tbody[i].childNodes;
                                
                                
                                [].forEach.call(informatii_meci,
                                    function(meci){
                                        if (meci.className) {
                                            meci_fotbal_rec.add_info(meci.className,meci.innerText);
                                            meci_fotbal_rec.site = "STANLEY";
                                        }
                                });
                                
                                
                                // odata gasit tr'ul se iese din loop
                                // am procedat asa in cazul in care nu gasesc tr sa nu bag balarii in DB
                                break;
                            } 
                        }
                        
                        //console.debug(meci_fotbal_rec.to_text());
                        //console.debug(meci_fotbal_rec.upload_to_db());
                        meci_fotbal_rec.upload_to_db();

                        break;
                        //se iese din loop
                    }
                    
                }
            
        );
}