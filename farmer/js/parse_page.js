
var getTableContent = function  () {
	console.debug("listener fired.");
        
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
                        console.debug(table_nodes[i].nodeName);
                        
                        // se citesc copii din tbody
                        var tbody = table_nodes[i].childNodes;
                        
                        for (var j=0,n_tbody=tbody.length;j<n_tbody;j++) {
                            if (tbody[i].nodeName.toLowerCase() == "tr") {
                                // pe tr se gasesc toate informatiile despre meci in copii de tip td
                                
                                var informatii_meci = tbody[i].childNodes;
                                
                                [].forEach.call(informatii_meci,
                                    function(meci){
                                        if (meci.className) {
                                            console.debug(meci.className);
                                            console.debug(meci.innerText);
                                        }
                                });
                                
                                // odata gasit tr'ul se iese din loop
                                // am procedat asa in cazul in care nu gasesc tr sa nu bag balarii in DB
                                break;
                            } 
                        }
                        
                        

                        break;
                        //se iese din loop
                    }
                    
                }
        );
        
       
/*        var table = query[0];
            
        console.debug(table.childNodes.length);
            
        var table_nodes = table.childNodes;
        
        for (var i=0,n=table_nodes.length;i<n;i++) {
            if (table_nodes[i].nodeName.toLowerCase() != "tbody") continue;
            
            // se citesc copii din tbody
            var meciuri = table_nodes[i].childNodes;
            
            [].forEach.call(meciuri,
                function(meci){
                    if (meci.className == "cod") 
                        console.debug(meci.value);
                });
                
            break;
            //se iese din loop
        }
    */        

}



chrome.runtime.onMessage.addListener(
	function(request, sender, sendResponse) {
		if (request.is_content_script)
			getTableContent();
  	});