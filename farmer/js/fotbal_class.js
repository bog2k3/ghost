/*
 * Data : 30-septembrie-2015
 * Programator : dan
 * 
 */

//fotbal momentan stanley

function meci_fotbal() {
    
    this.echipa1="";
    this.echipa2="";
    this.cota_1="";
    this.cota_2="";
    this.cota_x="";
    this.cota_1x="";
    this.cota_2x="";
    this.cota_12="";
    this.data="";
    
    this.add_info = function(header,text) {
        switch (header) {
            case "cota1" :
                //remove new line & shit
                this.cota_1=text.replace(/(\r\n|\n|\r)/gm,"");
                break;
            case "cotax":
                //remove new line & shit
                this.cota_x=text.replace(/(\r\n|\n|\r)/gm,"");
                break;
            case "cota2":
                //remove new line & shit
                this.cota_2=text.replace(/(\r\n|\n|\r)/gm,"");
                break;
            case "cota1x":
                //remove new line & shit
                this.cota_1x=text.replace(/(\r\n|\n|\r)/gm,"");
                break;
            case "cotax2":
                //remove new line & shit
                this.cota_2x=text.replace(/(\r\n|\n|\r)/gm,"");
                break;
            case "cota12":
                //remove new line & shit
                this.cota_12=text.replace(/(\r\n|\n|\r)/gm,"");
                break;
            case "echipe" :
                
                var data;
                var echipe;
                
                // primele 16 caractere reprezinta data
                data = text.replace(/(\r\n|\n|\r)/gm,"").slice(0,16);
                
                //restul echipele
                //echipele se impart dupa :
                echipe = text.replace(/(\r\n|\n|\r)/gm,"").slice(16).split(":");
                
                this.data=data;
                this.echipa1=echipe[0];
                this.echipa2=echipe[1];
                break;
        }
    }
    
    this.to_text = function() {
        return "datra= "+this.data+" echipe1= "+this.echipa1+" echipa2= "+this.echipa2+
               " Cota1= "+this.cota_1+" Cota2= "+this.cota_2+
               " Cotax= "+this.cota_x+" Cota12= "+this.cota_12+
               " Cota1x= "+this.cota_1x+" Cota2x= "+this.cota_2x+
               " Cota12= "+this.cota_12;
    }

}