
String data;

void setup(){
    pinMode(13, OUTPUT);
    Serial.begin(115200);
}

void loop(){

    if(Serial.available()){
        data = Serial.readString();

        /*
        first set of flashes indicates the target. second set indicates setting.

        f

        1 flash means setting closed, 2 flashes means setting open.
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
