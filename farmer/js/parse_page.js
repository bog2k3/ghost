


var getTableContent = function  (page_URL) {
        
        // stanley
        if (page_URL.toLowerCase().indexOf("www.stanleysports.ro/") > 0)
        {

            getStanley_Fotbal();
            
            chrome.runtime.sendMessage({site: "STANLEYBET"}, function(response) {
                
                if (response.refresh == "true" ) {
                    //console.debug("trebuie refresh");
                        
                    if (farmer_online==true) {
                        
                        //refresh page
                        
                        setTimeout(function() {
                                changeStanley_day(response.day);
                        },refresh_rate);
                    }
                } else {
                    
                    //console.debug("nu trebuie refresh");
                    
                    if (response.day!=0)
                        changeStanley_day(response.day);
                }
                
                
            });
            
        } else
        
        // betcafe
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
        } else
        
        // superbet
        if (page_URL.toLowerCase().indexOf("www.superbet.ro/") > 0) {
            
            //console.debug(""+page_URL);
            
            getSuperBet_Fotbal();
            
            chrome.runtime.sendMessage({site: "SUPERBET"}, function(response) {
                
                if (response.refresh == "true" ) {
                    //console.debug("trebuie refresh");
                        
                    if (farmer_online==true) {
                        
                        //refresh page
                        
                        setTimeout(function() {
                                changeSuperBet_day(response.day);
                        },refresh_rate);
                    }
                } else {
                    
                    //console.debug("nu trebuie refresh");
                    
                    if (response.day!=0)
                        changeSuperBet_day(response.day);
                }
                
                
            });
        } else

        // public bet
        if (page_URL.toLowerCase().indexOf("www.publicbet.ro/") > 0) {
            
            getPublicBet_Fotbal();
            
            chrome.runtime.sendMessage({site: "PUBLICBET"}, function(response) {
                
                if (response.refresh == "true" ) {
                    //console.debug("trebuie refresh");
                        
                    if (farmer_online==true) {
                        
                        //refresh page
                        
                        setTimeout(function() {
                                getPublicBet_Fotbal();
                        },refresh_rate);
                    }
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
