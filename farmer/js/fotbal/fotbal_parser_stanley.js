/*
 * Data : 30-septembrie-2015
 * autor : dan
 */

// http://www.stanleysports.ro/oferta.php?f=fotbal&z=1 - aici incepem

var changeStanley_day = function ( day ) {
    
    console.debug(day);
    
    // se extrag butoanele de zile
    var butoane_zile_main_div = document.getElementById("menu4");
    
    // primul element din lista e un text si ultimul lafel asa ca incepem la +1
    var buton = butoane_zile_main_div.childNodes[day+1];
    
    // click pe fraier
    setTimeout(function() {
        buton.click();
    },3000);
}


var getStanley_Fotbal = function  () {        
        
        //console.debug("listener fired.");
        
        // pentru stanley bet
        // meciurile sunt in tabele ce au clasa pariu
        
        var query = document.querySelectorAll('table.pariu');
        
        //console.debug(query.length);
        
        
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
                            // aici era i
                            if (tbody[j].nodeName.toLowerCase() == "tr") {
                                // pe tr se gasesc toate informatiile despre meci in copii de tip td
                                // 
                                // aici era tot i 
                                var informatii_meci = tbody[j].childNodes;
                                
                                
                                [].forEach.call(informatii_meci,
                                    function(meci){
                                        if (meci.className) {
                                            meci_fotbal_rec.add_info_stanley(meci.className,meci.innerText);
                                        }
                                });
                                
                                
                                // odata gasit tr'ul se iese din loop
                                // am procedat asa in cazul in care nu gasesc tr sa nu bag balarii in DB
                                break;
                            } 
                        }
                        
                        if (enable_debug==true)
                            console.debug(meci_fotbal_rec.to_text());
                        //console.debug(meci_fotbal_rec.upload_to_db());
                        meci_fotbal_rec.upload_to_db();

                        break;
                        //se iese din loop
                    }
                    
                }
            
        );
}