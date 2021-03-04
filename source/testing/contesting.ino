
String data;

void setup(){
    pinMode(13, OUTPUT);
    Serial.begin(115200);
}

void flash_once(){
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
}

void flash_twice(){
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
}

void loop(){

    digitalWrite(13, LOW);

    if(Serial.available()){
        data = Serial.readString();

        /*
        First set of flashes indicates the target. Second set of flashes indicates setting.

        If there's only one set of flashes, it's the igniter.

        1 flash means setting closed, 2 flashes means setting open.

        First set:
            0 flashes - igniter
            1 flashes - MEV
            2 flashes - N2OV
            3 flashes - N2O
            4 flashes - N2
            5 flashes - NCV
            6 flashes - RV
            7 flashes - N2V
        */

        //igniter control flow here

        if(data == "MEV closed"){ //1
            digitalWrite(13, LOW);

            flash_once();
            delay(1000); //gap between sets of flashes

            flash_once();
            delay(3000); //gap before next first set of flashes
        }

        else if(data == "MEV open"){ 
            digitalWrite(13, LOW);
            
            flash_once();
            delay(1000);
            
            flash_twice();
            delay(3000);
        }

        else if(data == "N2OV closed"){ //2
            digitalWrite(13, LOW);

            flash_once();
            flash_once();
            delay(1000);

            flash_once();
            delay(3000);
        }

        else if(data == "N2OV open"){ 
            digitalWrite(13, LOW);
            
            flash_once();
            flash_once();
            delay(1000);
            
            flash_twice();
            delay(3000);
        }

        else if(data == "N2O closed"){ //3
            digitalWrite(13, LOW);

            for(int i = 1; i <=3; i++) flash_once();
            delay(1000);

            flash_once();
            delay(3000);
        }

        else if(data == "N2O open"){ 
            digitalWrite(13, LOW);
            
            for(int i = 1; i <=3; i++) flash_once();
            delay(1000);
            
            flash_twice();
            delay(3000);
        }

        else if(data == "N2 closed"){ //4
            digitalWrite(13, LOW);

            for(int i = 1; i <=4; i++) flash_once();
            delay(1000);

            flash_once();
            delay(3000);
        }

        else if(data == "N2 open"){ 
            digitalWrite(13, LOW);
            
            for(int i = 1; i <=4; i++) flash_once();
            delay(1000);
            
            flash_twice();
            delay(3000);
        }

        else if(data == "NCV closed"){ //5
            digitalWrite(13, LOW);

            for(int i = 1; i <=5; i++) flash_once();
            delay(1000);

            flash_once();
            delay(3000);
        }

        else if(data == "NCV open"){
            digitalWrite(13, LOW);
            
            for(int i = 1; i <=5; i++) flash_once();
            delay(1000);
            
            flash_twice();
            delay(3000);
        }

        else if(data == "RV closed"){ //6
            digitalWrite(13, LOW);

            for(int i = 1; i <=6; i++) flash_once();
            delay(1000);

            flash_once();
            delay(3000);
        }

        else if(data == "RV open"){
            digitalWrite(13, LOW);
            
            for(int i = 1; i <=6; i++) flash_once();
            delay(1000);
            
            flash_twice();
            delay(3000);
        }

        else if(data == "N2V closed"){ //7
            digitalWrite(13, LOW);

            for(int i = 1; i <=7; i++) flash_once();
            delay(1000);

            flash_once();
            delay(3000);
        }

        else if(data == "N2V open"){
            digitalWrite(13, LOW);
            
            for(int i = 1; i <=7; i++) flash_once();
            delay(1000);
            
            flash_twice();
            delay(3000);
        }

    }

}
