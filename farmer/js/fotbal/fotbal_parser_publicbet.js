/*
 * Data : 11-octombrie-2015
 * autor : dan
 */

// http://www.publicbet.ro/osport;sport=1 - de aici se porneste

var changePublicBet_day = function ( day ) {
    return;
}

var getPublicBet_Fotbal = function  () {
    
    // in divurile astea sunt meciurile
    var div_ligi = document.querySelectorAll('table.offrzin');
    
    //console.debug(div_ligi[0]);
    
    // un tabel ce contine informatiile
    var table = div_ligi[0];
    
    // inca un tabel
    var table2 = table.childNodes[1];
    
    // var elementele din table
    var game_info = table2.childNodes;
    var n = game_info.length;
    for (var i=0;i<n;i++) {
        if (game_info[i].id) {
            var meci_fotbal_rec = new meci_fotbal();
            meci_fotbal_rec.add_info_publicbet(game_info[i]); 
            
            if (enable_debug==true)
                    console.debug(meci_fotbal_rec.to_text());
            //console.debug(meci_fotbal_rec.upload_to_db());
            //meci_fotbal_rec.upload_to_db();
        }
            
    }
    
    //console.debug(table.childNodes[1].childNodes);
    
    
    
    /*[].forEach.call( div_ligi, 
		function  fn(table){
                
                });
    */
}