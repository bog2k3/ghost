


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
            
            console.debug(""+page_URL);
            
            getBetcafearena_Fotbal();
            if (farmer_online==true) {
                //refresh page
                setTimeout('',refresh_rate);
                window.location.reload();
            }
        }
}

chrome.runtime.onMessage.addListener(
	function(request, sender, sendResponse) {
		if (request.is_content_script) {
			getTableContent(request.page_URL);
                }
  	});
