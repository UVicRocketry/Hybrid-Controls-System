/* UVR Propulsion Development Platform Mission Control Box Firmware
  Matthew Ebert
  2023-FEB-01

  This firmware controls the mission control box, gathering input from the switches and buttons, sending them to the controls computer

  This program only sends data on a STATE CHANGE.

  Limited logic should be implemented here.
  The idea is to create a simple controller for the attached computer (MCC)
*/


/***************DEFINITIONS ******************/


#define pinBUZZER 11


#define pinABORT 2
#define maskABORT (1<<pinABORT)

#define pinARMKEY 3
#define maskARMKEY 1<<pinARMKEY

#define pinIGFIRE 4
#define pinIGPRIME 5
#define maskIGPRIME (1<<pinIGPRIME)

#define pinMEV 6




#define numValves 10
String listValves[numValves] = {"IGFIRE", "IGPRIME", "MEV", "S1", "EVV", "NCV", "RTV", "N2F", "N2OV", "N2OF"};


#define maskPC 0x3F
#define maskPD 0xF0


enum status_t {
  DISARMED,//0
  ARMED,//1
};

volatile bool ABORT = false;
volatile uint8_t updateState = 0;
//volatile status_t STATUS = NULL;

//FUNCTION DECLARATION
uint16_t getState(void);

void setup() {
  Serial.begin(9600);
  while (!Serial) {};
  Serial.print("MCB,STATUS,STARTUP,\n");

  //sets pinmodes
  DDRD = 0x00;
  DDRC = 0x00;
  DDRB |= 0x04;
  analogWrite(11, 0);
  buzzerSetup();

  PCICR |= B00000100; // Enable interrupts on PD port
  PCMSK2 |= B00100000; // Trigger interrupts on pins D4 and D5

  //attachInterrupt(digitalPinToInterrupt(pinARMKEY), ISR_ARMINGKEY, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinABORT), ISR_ABORT, CHANGE);
  delay(100);

}//setup




void loop()
{

  uint16_t CurState = getState();
  uint16_t NewState = CurState;
  status_t CurSTATUS = getSTATUS();
  if (CurSTATUS == DISARMED) Serial.print("MCB,STATUS,DISARMED,\n");
  if (CurSTATUS == ARMED) Serial.print("MCB,STATUS,ARMED,\n");
  status_t NewSTATUS = CurSTATUS;

  while (!ABORT)
  {
    NewSTATUS = getSTATUS();
    delay(50);//debounce
    if (NewSTATUS == getSTATUS())
    {
      if (NewSTATUS != CurSTATUS)
      {
        CurSTATUS = NewSTATUS;
        if (CurSTATUS == DISARMED) Serial.print("MCB,STATUS,DISARMED,\n");
        if (CurSTATUS == ARMED) Serial.print("MCB,STATUS,ARMED,\n");
      }//IF CHANGE STATUS
    }//IF DEBOUNCE

    if (CurSTATUS)
    {
      NewState = getState();
      delay(50);//debounce
      if (NewState == getState())
      {
        if ((NewState != CurState))
        {
          if (getSTATUS())
          {
            sendStates(NewState);
            CurState = NewState;
          }
        }//IF CHANGE STATE
      }//IF DEBOUNCE
    }//IF STATUS
  }



  while (1)
  {
    Serial.print("MCB,ABORT,\n");
    delay(100);
  }

}//loop


//Functions
//-----------------------------------------------------------------------------------------------


uint16_t getState(void)
{
  uint16_t state = ((PIND & maskPD) >> 4) + ((PINC & maskPC) << 4);
  return state;
}


status_t getSTATUS(void)
{
  if ((PIND & maskARMKEY) == 0)
  {
    return ARMED;
  } else
  {
    return DISARMED;
  }
}



void sendStates(uint16_t tempState)
{
  Serial.print("MCB,CTRL,");

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





void buzzerSetup(void)
{
  cli();

  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 30000;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  // TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();
}



ISR (PCINT0_vect) {
  // code to execute
}


ISR(TIMER1_COMPA_vect)
{
  analogWrite(11, 0);
  TCCR1B &= !(1 << CS12) & !(1 << CS10); 
}


ISR (PCINT2_vect) {

  if ((PIND & maskIGPRIME) == 0)
  {
    analogWrite(11, 255);
    TCNT2 = 0;
    TCCR1B |= (1 << CS12) | (1 << CS10);
  }
}

void ISR_ABORT(void)
{
  for (int i = 0; i < 1000; i++)
  {
    if (PIND & maskABORT)
    {
      return;
    }
  }
  if ((PIND & maskABORT) == 0)
  {
    ABORT = true;
    Serial.print("MCB,ABORT,\n");
    analogWrite(11, 255);
    TCNT2 = 0;
    TCCR1B |= (1 << CS12) | (1 << CS10);
  }
}
