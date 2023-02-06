String data;
boolean established=false;
String currentDataFocus;
String whoami="VC";

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(2);
}
int numOfSubstring(String inString){
  int ssCount=0;
  for(int i=0; i<inString.length(); i++){
    if(inString[i]==','){
      ssCount=ssCount+1;
    }
  }
  return ssCount+1;
}
String getSubstring(String inString, int subNumber){
  int divCount = 0;
  int start = 0;
  for(int i=0; i<inString.length(); i++){
    char c = inString[i];
    if(c==','){
      if (divCount==subNumber){
        return inString.substring(start,i);
      }else{
        start=i+1;
        divCount=divCount+1;
      }
    }
  }
  if(divCount<subNumber){
    return("Error");
  }else{
    return inString.substring(start,inString.length());
  }
  
}
void commandProcessing(String data){
  switch(numOfSubstring(data)) {
    case 2:
      if(getSubstring(data,0)=="MCC"&&getSubstring(data,1)=="CONNECT"){
          Serial.println("VC,STATUS,ESTABLISH");
          established=true;
        }
        else if(getSubstring(data,0)=="MCC"&&getSubstring(data,1)=="ARM"){
          //put arming code here
          Serial.println("VC,STATUS,ARMED");
        }
        else if(getSubstring(data,0)=="MCC"&&getSubstring(data,1)=="DISARM"){
          //put disarming code here
          Serial.println("VC,STATUS,DISARMED");
        }
        else if(getSubstring(data,0)=="MCC"&&getSubstring(data,1)=="PING"){
          //put disarming code here
          Serial.println("VC,PING");
        }
        else if(getSubstring(data,0)=="MCC"&&getSubstring(data,1)=="ABORT"){
          //put abort code here
          Serial.println("VC,STATUS,ABORTED");
        }
        break;
    case 3:
      if(getSubstring(data,0)=="MCC"&&getSubstring(data,1)=="GET"){
        Serial.println("VC,SWITCHSTATE,"+getSubstring(data,2)+",DUMMY");
      }
      break;
    case 4:
      if(getSubstring(data,0)=="MCC"&&getSubstring(data,1)=="CTRL"){
        Serial.println("VC,SWITCHSTATE,"+getSubstring(data,2)+","+getSubstring(data,3));
      }
    default:
      Serial.println("VC,ERROR,UNKNOWNCOMMAND");
      break;
}
      return;
}
void loop() {
  if(Serial.available()>0){
    data = "";
    data = Serial.readStringUntil('\n');
    data.trim();
    commandProcessing(data);
  }else if(established==false){
    Serial.println("VC,STATUS,STARTUP");
    delay(1000);
  }
}
