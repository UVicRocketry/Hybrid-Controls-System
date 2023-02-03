/* UVR Propulsion Development Platform Mission Control Box Firmware
  Matthew Ebert
  2023-FEB-01

  This firmware controls the mission control box, gathering input from the switches and buttons, sending them to the controls computer

  This program only sends data on a STATE CHANGE.

  Limited logic should be implemented here.
  The idea is to create a simple controller for the attached computer (MCC)
*/


/***************DEFINITIONS ******************/

#define pinN2OF 4
#define pinN2OV 5
#define pinN2V 6


#define pinABORT 2
#define maskABORT (1<<pinABORT)

#define pinARMKEY 3
#define maskARMKEY 1<<pinARMKEY



#define numValves 3
String listValves[numValves] = { "N2OF", "N2OV", "N2F"};


#define maskPB 0x1F
#define maskPD 0xFC


enum state {
  ABORT,  //0
  DISARMED,//1
  ARMED,//2
  PRIMED,//3
  FIRE//4
};


volatile uint8_t updateState = 0;
volatile state STATUS = NULL;

//FUNCTION DECLARATION
uint16_t getState(void);

void setup() {
  Serial.begin(9600);
  while (!Serial) {};
  Serial.print("MCB,STATUS,STARTUP,\n");

  //sets pinmodes
  DDRD = 0x00;
  DDRB = 0x00;

  attachInterrupt(digitalPinToInterrupt(pinARMKEY), ISR_ARMINGKEY, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinABORT), ISR_ABORT, CHANGE);


  delay(100);
  STATUS = DISARMED;

  if ((getState() & maskARMKEY) == 0)
  {
    Serial.print("MCB,STATUS,ARMED,\n");
    STATUS = ARMED;
  } else
  {
    Serial.print("MCB,STATUS,DISARMED,\n");
    STATUS = DISARMED;
  }

}//setup




void loop()
{

  uint16_t CurState = getState();
  uint16_t NewState = CurState;


  while (STATUS)
  {



    if (updateState)
    {
      if (STATUS == DISARMED) Serial.print("MCB,STATUS,DISARMED,\n");
      if (STATUS == ARMED) Serial.print("MCB,STATUS,ARMED,\n");
      updateState = 0;
    }

    NewState = getState();
    delay(50);//debounce
    if (NewState == getState())
    {
      if ((NewState != CurState) && (STATUS > 1))
      {
        sendStates(NewState);
        CurState = NewState;
      }
    }
  }

  while (1)
  {
    delay(100);
    Serial.print("MCB,ABORT,\n");
  }
}//loop


//Functions
//-----------------------------------------------------------------------------------------------


void sendStates(uint16_t tempState)
{
  Serial.print("MCB,CTRL,");
  tempState = tempState >> 4;
  for (int i = 0; i < numValves; i++)
  {
    Serial.print(listValves[i]);
    if (tempState & 0x01)
    {
      Serial.print(",CLOSE,");
    } else
    {
      Serial.print(",OPEN,");
    }
    tempState = tempState >> 1;
  }
  Serial.print("\n");
}

uint16_t getState(void)
{
  uint16_t state = ((PIND & maskPD)) + ((PINB & maskPB) << 8);
  return state;
}


void ISR_ARMINGKEY(void)
{
  delay(100);//debounce

  uint8_t valARMKEY  = (PIND & maskARMKEY);

  if ((valARMKEY == 0) && (STATUS != ARMED))
  {
    STATUS = ARMED;
    updateState = 1;
  } else if ((valARMKEY > 0) && (STATUS != DISARMED))
  {
    STATUS = DISARMED;
    updateState = 1;
  }


}


void ISR_ABORT(void)
{
  delay(100);
  if ((PIND & maskABORT) == 0)
  {
    STATUS = ABORT;
  }
}
