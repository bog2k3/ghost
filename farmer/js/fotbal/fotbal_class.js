/*
 * Data : 30-septembrie-2015
 * autor : dan
 *
 * Dan : 6-octombrie-2015   : adaugare betcafearena
 *                          : bug - nu adaugam si data meciului
 */

// stanley
// betcafearena

function meci_fotbal() {
    
    this.echipa1="";
    this.echipa2="";
    this.cota_1="";
    this.cota_2="";
    this.cota_x="";
    this.cota_1x="";
    this.cota_2x="";
    this.cota_12="";
    this.data=""; // data trebuie formatata ca pe stanleybet zz/ll/aaaa
    this.cod="";
    this.site="";
    
    this.add_info_betcafearena = function (header,body) {
        // ca si input se primeste un heaer de tip tr ce are mai multe TD'uri reprezentand textul cu cote si informatii
        // si un body (tr) cu mai multe td'uri fiecare td reprezentand o informatie despre meci
        
        this.site = "BETCAFEARENA";
        
        // body dimensiune - 18
        // header dimensiune - 15  - ultima coloana este blank
        
        var n = body.length;
        var m = header.length;
        
        if (n-m != 3) return;
        
        // se parseaza body'ul
        
        // prima coloana codul
        this.cod=body[0].innerText.replace(/(\r\n|\n|\r)/gm,"");
        
        // a doua coloana echipele
        // dobitocii nu au facut si ei doua coloane MUIEEEE
        var echipe_split = body[1].innerText.split(" v ");
        
        this.echipa1 = echipe_split[0].replace(/(\r\n|\n|\r)/gm,"");
        this.echipa2 = echipe_split[1].replace(/(\r\n|\n|\r)/gm,"");
        
        // data
        // coloana 4
        // scapam de minus animalele nu au fost in stare sa faca si ei o data calumea
        var data_split = body[3].innerText.replace(/(\r\n|\n|\r)/gm,"").split(' - ');
        // mai intai imi vine luna apoi ziua si fara an PLM
        // Return today's date and time
        var currentTime = new Date();

        
        var bucati_data = data_split[0].split('/');
        // daca ziua are o singura cifra adaugam un zero in fata
        if (bucati_data[1].length == 1) 
            bucati_data[1] = "0"+bucati_data[1];
        
        this.data = bucati_data[1]+"/"+bucati_data[0]+"/"+currentTime.getFullYear();
        
        //trecem la cote
        
        // pentru ca mi-e lene folosesc variabile pentru indeci... sper sa imi aduc aminte
        // ce am facut aici
        // trebuie +2 pentru ca pe langa tr'uri si th'uri mai sunt si niste chestii #text PLM ce sunt alea
        
        for (var header_index = 2;header_index<14;header_index+=2) {
            // coloana 5 prima cota
            
            switch (header[header_index].innerText.toLowerCase().replace(/(\r\n|\n|\r)/gm,"")) {
                case "1":
                    this.cota_1=body[header_index+3].innerText.replace(/(\r\n|\n|\r)/gm,"");
                    break;
                case "2":
                    this.cota_2=body[header_index+3].innerText.replace(/(\r\n|\n|\r)/gm,"");
                    break;
                case "x":
                    this.cota_x=body[header_index+3].innerText.replace(/(\r\n|\n|\r)/gm,"");
                    break;
                case "1x":
                    this.cota_1x=body[header_index+3].innerText.replace(/(\r\n|\n|\r)/gm,"");
                    break;
                case "x2":
                    this.cota_2x=body[header_index+3].innerText.replace(/(\r\n|\n|\r)/gm,"");
                    break;
                case "12":
                    this.cota_12=body[header_index+3].innerText.replace(/(\r\n|\n|\r)/gm,"");
                    break;
            }
        }
        
        

    }
    
    this.add_info_stanley = function(header,text) {
        this.site = "STANLEY";
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
                data = text.replace(/(\r\n|\n|\r)/gm,"").split(" ");
                
                //restul echipele
                //echipele se impart dupa :
                echipe = text.replace(/(\r\n|\n|\r)/gm,"").slice(16).split(":");
                
                this.data=data[0];
                this.echipa1=echipe[0];
                this.echipa2=echipe[1];
                break;
            case "cod" :
                this.cod= text.replace(/(\r\n|\n|\r)/gm,"");
                break;
        }
    }
    
    this.to_text = function() {
        return "data= "+this.data+" echipe1= "+this.echipa1+" echipa2= "+this.echipa2+
               " Cota1= "+this.cota_1+" Cota2= "+this.cota_2+
               " Cotax= "+this.cota_x+" Cota12= "+this.cota_12+
               " Cota1x= "+this.cota_1x+" Cota2x= "+this.cota_2x+
               " Cota12= "+this.cota_12+" data = "+this.data+" cod= "+this.cod;
    }
    
    this.upload_to_db = function () {
        
        // se creeaza requestul

        http_request = backend_php_server_protocol+"://"+
                       backend_php_server+":"+
                       backend_php_server_port+
                       backend_php_server_fotbal+
                       "?sesiune=1"+
                       "&e1="+this.echipa1+
                       "&e2="+this.echipa2+
                       "&c1="+this.cota_1+
                       "&c2="+this.cota_2+
                       "&cx="+this.cota_x+
                       "&c1x="+this.cota_1x+
                       "&c2x="+this.cota_2x+
                       "&c12="+this.cota_12+
                       "&site="+this.site+
                       "&game_id="+this.cod+
                       "&game_date="+this.data
                        ;
        $.getJSON(http_request,
            function(data) {
                return;
            });
        return http_request;
    }

}