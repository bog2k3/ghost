


var getTableContent = function  (page_URL) {
        
        
        if (page_URL.toLowerCase().indexOf("www.stanleysports.ro/") > 0)
        {
            //console.debug(""+page_URL);
            
            getStanley_Fotbal();
            
            //refresh page
            // treaba asta merge doar daca tabul este activ
            setTimeout('',refresh_rate);
            window.location.reload();
        }
}

chrome.runtime.onMessage.addListener(
	function(request, sender, sendResponse) {
		if (request.is_content_script) {
			getTableContent(request.page_URL);
                }
  	});
