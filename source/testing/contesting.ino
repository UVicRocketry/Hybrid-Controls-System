
String data;

void setup(){
    pinMode(13, OUTPUT);
    Serial.begin(115200);
}

void loop(){

    if(Serial.available()){
        data = Serial.readString();

        /*
        First set of flashes indicates the target. Second set of flashes indicates setting.

        1 flash means setting closed, 2 flashes means setting open.

        First set:
            0 flashes
            1 flashes - MEV
            2 flashes - N2OV
            3 flashes - N2O
            4 flashes - N2
            5 flashes - NCV
            6 flashes - RV
            7 flashes - N2V
        */

        //igniter control flow here

        if(data == "MEV_closed"){ //1
            digitalWrite(13, HIGH);
            delay(500);
            digitalWrite(13, LOW);
            delay(2500);
        }

        else if(data == "MEV_open"){ 
            digitalWrite(13, HIGH);
            delay(500);
            digitalWrite(13, LOW);
            delay(500);
            digitalWrite(13, HIGH);
            delay(500);
            digitalWrite(13, LOW);
            delay(2500);
        }

        else if(data == "N2OV_closed"){ //2
            
        }

        else if(data == "N2OV_open"){ 
            
        }

        else if(data == "N2O_closed"){ //3
            
        }

        else if(data == "N2O_open"){ 
            
        }

        else if(data == "N2_closed"){ //4
            
        }

        else if(data == "N2_open"){ 
            
        }

        else if(data == "NCV_closed"){ //5
            
        }

        else if(data == "NCV_open"){
            
        }

        else if(data == "RV_closed"){ //6
            
        }

        else if(data == "RV_open"){
            
        }

        else if(data == "N2V_closed"){ //7
            
        }

        else if(data == "N2V_open"){
            
        }

    }

}
