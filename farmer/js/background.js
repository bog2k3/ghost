
function updatePageMessage(tabId) 
{
	chrome.tabs.query({active: true, currentWindow: true}, function() {
  		chrome.tabs.sendMessage(tabId, {is_content_script:true});
  		});
}

chrome.tabs.onUpdated.addListener(
function(tabId,changeInfo,tab){
	if (changeInfo.status == 'complete' && tab.active) {
		updatePageMessage(tabId);
	}
});
