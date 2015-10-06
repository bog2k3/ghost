
function updatePageMessage(tabId,tab) 
{
	chrome.tabs.query({active: false, currentWindow: false}, function() {
  		chrome.tabs.sendMessage(tabId, {is_content_script:true,page_URL:tab.url});
  		});
}

chrome.tabs.onUpdated.addListener(
function(tabId,changeInfo,tab){
    
        // am scos asta pentru a refresui si taburile non active
        //&& tab.active
	if (changeInfo.status == 'complete') {
		updatePageMessage(tabId,tab);
	}
});

