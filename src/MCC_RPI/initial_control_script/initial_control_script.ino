const unsigned long TIMEOUT = 2000;


int executeCommand(String target, int value);
int setN20Vent(int value);
int setN20Flow(int value);

void setup() {
  Serial.begin(9600);
  Serial.flush();
  // Insert pin setup code etc.

}

void loop() {
  unsigned long startWait = millis();
  while (Serial.available() == 0){
    if(millis() - startWait > TIMEOUT){
      //check if abort is necessary (i.e. igniter is going)
      //if so abort
      Serial.println("Timeout reached");
      startWait = millis();
    }
  }
  // COMMAND FORMAT: VALVE:[-1|1]
  String target = Serial.readStringUntil(':'); //the name of the valve/ignitor
  int value = Serial.readStringUntil('\n').toInt(); // either a 1 or a -1 for open or closed
  Serial.println(target);
  if (!target.equals("Ping")){
    int success = executeCommand(target, value);
    if (success){
      Serial.println("Success");
    } else {
      Serial.println("Failure");
    }
  }
}

int executeCommand(String target, int value){
  if (target.equals("N20Flow")){
    return setN20Flow(value);
  }
  else if (target.equals("N20Vent")){
    return setN20Vent(value);
  }
  //repeat for each valve/ignitor
  return 1;
}

int setN20Flow(int value){
  //try to set valve, return 0 if failure, return 1 if success
  return 1;
}

int setN20Vent(int value){
  //try to set valve, return 0 if failure, return 1 if success
  return 1;
}
