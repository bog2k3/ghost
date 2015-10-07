


var getTableContent = function  (page_URL) {
        
        
        if (page_URL.toLowerCase().indexOf("www.stanleysports.ro/") > 0)
        {
            //console.debug(""+page_URL);
            
            getStanley_Fotbal();
            
            if (farmer_online==true) {
                //refresh page
                setTimeout('',refresh_rate);
                window.location.reload();
            }
        }
        
        if (page_URL.toLowerCase().indexOf("www.betcafearena.ro/") > 0) {
            
            //console.debug(""+page_URL);
            
            getBetcafearena_Fotbal();
            
            chrome.runtime.sendMessage({site: "BETCAFEARENA"}, function(response) {
                
                if (response.refresh == "true" ) {
                    //console.debug("trebuie refresh");
                        
                    if (farmer_online==true) {
                        
                        //refresh page
                        
                        setTimeout(function() {
                                changeBetcafearena_day(response.day);
                        },refresh_rate);
                    }
                } else {
                    
                    //console.debug("nu trebuie refresh");
                    
                    if (response.day!=0)
                        changeBetcafearena_day(response.day);
                }
                
                
            });
        }
}

chrome.runtime.onMessage.addListener(
	function(request, sender, sendResponse) {
		if (request.is_content_script) {
			getTableContent(request.page_URL);
                }
  	});
