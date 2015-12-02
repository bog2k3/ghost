
// variabile pentru betcafe
var betcafearena_day=0;

// variabile pentru stanley
var stanleybet_day=0;

//variabile pentru publicbet
// nu trebuie variabila de zi pentru ca baietii pun meciurile pe toata saptamana
// asa ma frends
//var publicbet_day=0;

//variabile pentru superbet
var superbet_day=0;

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

chrome.runtime.onMessage.addListener(
function(message,sender,sendResponse){
    
    // verificam de pe ce tab a venit mesajul
    if (message.site === "BETCAFEARENA") {
        betcafearena_day++;
        if (betcafearena_day > 8) {
            betcafearena_day=0;
            sendResponse({refresh:"true",day:betcafearena_day});
        } else {
            sendResponse({refresh:"false",day:betcafearena_day});
        }
    } else
    if (message.site === "STANLEYBET") {
        stanleybet_day++;
        if (stanleybet_day > 6) {
            stanleybet_day=0;
            sendResponse({refresh:"true",day:stanleybet_day});
        } else {
            sendResponse({refresh:"false",day:stanleybet_day});
        }
    } else
    if (message.site === "PUBLICBET") {
         sendResponse({refresh:"true"});
    } else
    if (message.site === "SUPERBET") {
        superbet_day++;
        if (superbet_day > 6) {
            superbet_day=0;
            sendResponse({refresh:"true",day:superbet_day});
        } else {
            sendResponse({refresh:"false",day:superbet_day});
        }
    } 
    
});

