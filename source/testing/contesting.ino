
String data;

void setup(){
    pinMode(13, OUTPUT);
    Serial.begin(115200);
}
//hello this is a comment
void loop(){

    if(Serial.available()){
        data = Serial.readString();

        if(data == "MEV open"){
            digital.Write(13, HIGH);
            delay(1000);
            digital.Write(13, LOW);
        }

    }

}