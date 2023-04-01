/* UVR Propulsion Development Platform Mission Control Box Firmware
  Matthew Ebert
  2023-FEB-01

  This firmware controls the mission control box, gathering input from the switches and buttons, sending them to the controls computer

  This program only sends data on a STATE CHANGE.

  Limited logic should be implemented here.
  The idea is to create a simple controller for the attached computer (MCC)
*/


/***************DEFINITIONS ******************/


/*************PINS and MASKS**************/
#define pinBUZ 11
#define pinABORT 2
#define maskABORT (1<<pinABORT)
#define pinARMKEY 3
#define maskARMKEY 1<<pinARMKEY
#define pinIGFIRE 4
#define maskIGFIRE (1<<pinIGFIRE)
#define pinIGPRIME 5
#define maskIGPRIME (1<<pinIGPRIME)
#define maskPC 0x3F
#define maskPD 0xF0



/**************STRING OUTPUT FORMAT*******************/
#define numValves 10
String listValves[numValves] = {"MEV", "IGPRIME", "IGFIRE", "EVV", "NCV", "RTV", "N2F", "N2OV", "N2OF", "S1"};


/****************TYPES******************/
enum status_t {
  DISARMED,//0
  ARMED,//1
};


/**************GLOBALS****************/

volatile bool ABORT = false;
volatile status_t CurSTATUS = NULL;
volatile uint16_t CurState = NULL;




void setup() {
  Serial.begin(9600);
  
  while (!Serial) {};

  Serial.setTimeout(100);
  Serial.flush();
  Serial.print("MCB,STATUS,STARTUP,\n");

  cli();
  //sets pinmodes
  DDRD = 0x00;
  DDRC = 0x00;
  DDRB |= 0x04;
  digitalWrite(11, 0);
  abortSetup();
  buzzerSetup();

  PCICR |= B00000100; // Enable interrupts on PD port
  PCMSK2 |= B00101000;
  sei();
  delay(100);

}//setup




void loop()
{

  CurState = getState();
  uint16_t NewState = CurState;
  CurSTATUS = getSTATUS();
  if (CurSTATUS == DISARMED) Serial.print("MCB,STATUS,DISARMED,\n");
  if (CurSTATUS == ARMED)
  {
    Serial.print("MCB,STATUS,ARMED,\n");
    sendStates(NewState);
  }
  status_t NewSTATUS = CurSTATUS;



  while (!ABORT)
  {


    if (Serial.read() > 0)
    {
      if (CurSTATUS == DISARMED)
      {
        if (CurSTATUS == DISARMED) Serial.print("MCB,STATUS,DISARMED,\n");
        return;
      } else
      {
        if (CurSTATUS == ARMED) Serial.print("MCB,STATUS,ARMED,\n");
        sendStates(CurState);
      }
   
    }

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

    
  }//WHILE !ABORT




//When ABORT PRESSED
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
  //Serial.print("Registers: ");
  //Serial.println(state);
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



void abortSetup(void)
{

  EIMSK |= 0x01;//Enable INT0 on D2
  EICRA |= _BV(ISC00) | _BV(ISC00);

}

void buzzerSetup(void)
{
 
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 10000;
  // turn on CTC mode
  TCCR1B |= _BV(WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  // TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= _BV(OCIE1A);
}


ISR(TIMER1_COMPA_vect)
{
  analogWrite(11, 0);
  TCCR1B &= !_BV(CS12) & !_BV(CS10);
}


ISR (PCINT2_vect) {
  delay(50);
  if ((PIND & maskIGPRIME) == 0)
  {
    digitalWrite(pinBUZ, 1);
    TCNT1 = 0;
    TCCR1B |= _BV(CS12) | _BV(CS10);
  }

  
  if (((PIND & maskARMKEY) == 0) && (CurSTATUS == DISARMED))
  {
    digitalWrite(pinBUZ, 1);
    TCNT1 = 0;
    TCCR1B |= _BV(CS12) | _BV(CS10);
  }
}


ISR(INT0_vect)
{

  for (int i = 0; i < 1000; i++)
  {
    if ((PIND & maskABORT) == 0)
    {
      return;
    }
  }
  if ((PIND & maskABORT) > 0)
  {
    ABORT = true;
    Serial.print("MCB,ABORT,\n");
    digitalWrite(pinBUZ, 1);
    TCNT1 = 0;
    TCCR1B |= _BV(CS12) | _BV(CS10);
  }
}
