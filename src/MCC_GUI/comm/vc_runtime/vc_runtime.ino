String data;
boolean established=false;
String currentDataFocus;
String whoami="VC";

//define valve status bools
String valves[9][2] = {
{"N2OF", "UNKWN"},
{"N2OV", "UNKWN"},
{"N2F", "UNKWN"},
{"RTV", "UNKWN"},
{"NCV", "UNKWN"},
{"EVV", "UNKWN"},
{"IGPRIME", "UNKWN"},
{"IGFIRE", "UNKWN"},
{"MEV", "UNKWN"},
};

String LookupValve(String valveName){
  for(int i=0; i<=9; i++){
    if(valves[i][0]==valveName){
      return(valves[i][1]);
    }    
  }
}
void SetValve(String valveName, String newVal){
  for(int i=0; i<=9; i++){
    if(valves[i][0]==valveName){
      valves[i][1]=newVal;
    }    
  }
}
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
          //this is just example code to show updating status on the GUI
          delay(50);SetValve("N2OF", "OPEN");SetValve("N2OV", "OPEN");SetValve("N2F", "CLOSE");delay(100);SetValve("IGFIRE","ABRT");
          //put abort code here
          Serial.println("VC,STATUS,ABORTED");
        }
        else if(getSubstring(data,0)=="MCC"&&getSubstring(data,1)=="SUMMARY"){
          //put abort code here
          Serial.println("VC,SUMMARY,"+LookupValve("N2OF")+","+LookupValve("N2OV")+","+LookupValve("N2F")+","+LookupValve("RTV")+","+LookupValve("NCV")+","+LookupValve("EVV")+","+LookupValve("IGPRIME")+","+LookupValve("IGFIRE")+","+LookupValve("MEV"));
        }
        break;
    case 3:
      if(getSubstring(data,0)=="MCC"&&getSubstring(data,1)=="GET"){
        //put valve status check code here
        Serial.println("VC,SWITCHSTATE,"+getSubstring(data,2)+",DUMMY");
      }
      break;
    case 4:
      if(getSubstring(data,0)=="MCC"&&getSubstring(data,1)=="CTRL"){
        SetValve(getSubstring(data,2), getSubstring(data,3));
        //put valve move code here
        Serial.println("VC,SWITCHSTATE,"+getSubstring(data,2)+","+LookupValve(getSubstring(data,2)));
      }
      break;
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
