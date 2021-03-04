
String data;

void setup(){
    pinMode(13, OUTPUT);
    Serial.begin(115200);
}

void loop(){

    if(Serial.available()){
        data = Serial.readString();

        if(data == "MEV open"){
            digitalWrite(13, HIGH);
            delay(1000);
            digitalWrite(13, LOW);
        }

    }

}