//includes here
#include <SPI.h>
#include <DS3231.h>
#include <Wire.h>

//#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
//#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>


//defines here

#define RED_LED 2
//Shocker init
#define SHOCK 3



//includes here

//LCD stuff here
//LCD init
  #define TFT_CS        10
  #define TFT_RST       8
  #define TFT_DC         9
  #define LCD_ENABLE    4

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


//end LCD stuff

//RTC Stuff here
      DS3231 clock;
      bool century = false;
      bool h12Flag;
      bool pmFlag;
      byte alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits,Hour,Minute,Second;
      bool alarmDy, alarmH12Flag, alarmPmFlag;

      int oldTimeMinute;
      int oldTimeHour;
      //end RTC Stuff

//button Stuff here
int const POLL_INTERVAL = 200;           // interval at which to blink (milliseconds)
unsigned long previousMillis = 0;        // will store last time LED was updated
#define B1 5
#define B2 6
#define B3 7
int B1State = 0;         // variable for reading the pushbutton status
int B2State = 0;         // variable for reading the pushbutton status
bool B3State = 0;         // variable for reading the pushbutton status




//State/nextstate globals here
uint8_t state = 1;
uint8_t nextState =1;


void serialTime(){
Serial.print("2");
  if (century) {      // Won't need this for 89 years.
    Serial.print("1");
  } else {
    Serial.print("0");
  }
  Serial.print(clock.getYear(), DEC);
  Serial.print(' ');
  
  // then the month
  Serial.print(clock.getMonth(century), DEC);
  Serial.print(" ");
  
  // then the date
  Serial.print(clock.getDate(), DEC);
  Serial.print(" ");
  
  // and the day of the week
  Serial.print(clock.getDoW(), DEC);
  Serial.print(" ");
  
  // Finally the hour, minute, and second
  Serial.print(clock.getHour(h12Flag, pmFlag), DEC);
  Serial.print(" ");
  Serial.print(clock.getMinute(), DEC);
  Serial.print(" ");
  Serial.print(clock.getSecond(), DEC);
 
  // Add AM/PM indicator
  if (h12Flag) {
    if (pmFlag) {
      Serial.print(" PM ");
    } else {
      Serial.print(" AM ");
    }
  } else {
    Serial.print(" 24h ");
  }
 
  // Display the temperature
  Serial.print("T=");
  Serial.print(clock.getTemperature(), 2);
  
  // Tell whether the time is (likely to be) valid
  if (clock.oscillatorCheck()) {
    Serial.print(" O+");
  } else {
    Serial.print(" O-");
  }
 
  // Indicate whether an alarm went off
  if (clock.checkIfAlarm(1)) {
    Serial.print(" A1!");
  }
 
  if (clock.checkIfAlarm(2)) {
    Serial.print(" A2!");
  }
 
  // New line on display
  Serial.println();
  
  
  }

//shock routine here
void infiniteShockDeathSpiral(){


  Serial.println("Enters ISDS function");

        while(true){
             Serial.println("in shock state");
             analogWrite(SHOCK,007);
             digitalWrite(RED_LED,HIGH);
             delay(100);
    
             Serial.println("lull state");
             digitalWrite(SHOCK,LOW);
             digitalWrite(RED_LED,LOW);
             delay(5000);
              
              }
      
  }

//end shock routine


  char testVal[5];
//display utility routine
char* intToArray(int x){


  String stringVal;
  stringVal = String(x);
  stringVal.toCharArray(testVal,4);

  return testVal;
  }

//display routine here
//startDisplayTime
//displayFlag (don't update the display unless there's been a button press

uint8_t displayFlag = 1;


void displayTime(){

    if(displayFlag){
      Serial.println("actually made it");
          tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor( ST77XX_WHITE);
    tft.setTextWrap(true);    

    tft.print("state: ");
    tft.print(intToArray(state));
    tft.print("\n\r");

    tft.print("alarm hour: ");
    tft.print(intToArray(alarmHour));
    tft.print("\n\r");
    
    tft.print("alarm minute: ");
    tft.print(intToArray(alarmMinute));
    tft.print("\n\r");

    tft.print("time hour: ");
    tft.print(intToArray(Hour));
    tft.print("\n\r");

    tft.print("time minute: ");
    tft.print(intToArray(Minute));
    tft.print("\n\r");


    
    Serial.println("arrived at displayTime");
    displayFlag = 0;

    
      
      
      
      
      
      
      
      
      }
      
      }

    
  



//periodic button polling here

void pollButtons(){


  
    unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= POLL_INTERVAL) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    //do stuff here
          //button polling
     if(digitalRead(B1)){ //increment hour
          B1State =1;
          Serial.println("B1");
          displayFlag = 1;
     }
     if(digitalRead(B2)){ //increment minute
          B2State =1;
          Serial.println("B2");
          displayFlag = 1;
     }
     
     if(digitalRead(B3)){//toggle alarm,
            B3State =1;
            Serial.println("B3");
            displayFlag = 1;
          }
  }
}







void setup() {
  // put your setup code here, to run once:

  //LCD setup here

     pinMode(LCD_ENABLE,OUTPUT);
     digitalWrite(LCD_ENABLE,HIGH);
     tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
    
  //end LCD setup
  //RTC setup here
      pinMode(12, OUTPUT);
      digitalWrite(12,HIGH);//turn on the RTC chip
      Wire.begin();
     
  // Start the serial interface
      Serial.begin(9600);
      Serial.println("test serial");
  //end RTC setup

  //set time
      
      
     // clock.setClockMode(false);  // set to 24h
      //setClockMode(true); // set to 12h
       Hour = clock.getHour(h12Flag, pmFlag);
       Minute = clock.getMinute();
       Second = clock.getSecond();
      

  
    // clock.setHour(Hour);
     // clock.setMinute(Minute);
      //clock.setSecond(Second);
      
  //end set time

  //set alarm
      alarmDay = clock.getDoW();
     // alarmHour = 10;
        alarmHour = clock.getHour(h12Flag, pmFlag);
        alarmMinute = Minute + 2;
        alarmMinute %=60;
      //alarmMinute = 30;
      alarmSecond = 0 ;
      clock.setA1Time(clock.getDoW(), alarmHour, alarmMinute, alarmSecond, 0x0, true,  //DOW, Hour, Minute, Second, 
      false, false);
      clock.turnOnAlarm(1);
  
  //end set alarm

  //State setup

  //button setup
   pinMode(B1, INPUT);
   pinMode(B2, INPUT);
   pinMode(B3, INPUT);


  //other setup
      pinMode(SHOCK, OUTPUT);
      pinMode(RED_LED, OUTPUT);

}





int startSecond =0;

void loop() {
    pollButtons();

    
if(state == 1){
    //state 1 here
  
  //check the time. call shock routine if true 
  //set next state based on button press
        delay(100);
        //Serial.println("in state 1");
        
        if(B1State){
            Serial.println("setting state from 1 to 2");
            B1State =0; //reset
            displayFlag =1; //show 
            nextState = 2; //progress
            startSecond = clock.getSecond();//set this for timeout
            digitalWrite(LCD_ENABLE,HIGH);
            delay(1000);//give time for the clock to change and LCD to power on
          }else if(B2State ||B3State){
            Serial.println("setting state from 1 to 3");
            B2State =0; //reset
            B3State = 0;
            displayFlag =1; //show 
            nextState = 3; //progress to 3
            startSecond = clock.getSecond();//set this for timeout
            digitalWrite(LCD_ENABLE,HIGH);
               delay(1500);//give time for the clock to change and LCD to power on
            }
  
        //clock.getA1Time(alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
        /*
        Serial.print("hour variable");
        Serial.println(Hour);
        Serial.print("Minute variable");
        Serial.println(Minute);
        Serial.print("clock hour");
        Serial.println(clock.getHour(h12Flag, pmFlag));
        Serial.print("clock minute");
        Serial.println(clock.getMinute());
        */
        if ((clock.getHour(h12Flag, pmFlag) == alarmHour) && (clock.getMinute() == alarmMinute) )    {
              Serial.print(" A1!");
              infiniteShockDeathSpiral();
        }
  
  }


if(state == 2){
    //displayFlag =1; //show 
  delay(20);
    //state 2 here, display with no update. for button 3
  //call time display routine
  Serial.println("state 2 reached");
   displayTime();

   if(clock.getSecond() == startSecond){ //leave after a minute
    nextState = 1;
    digitalWrite(LCD_ENABLE,LOW);
    delay(500);//give time for the clock to change and LCD to power on
       Serial.println("going back to 1 from 2");
       displayFlag = 1;
    }

    if(B2State ||B3State){ //if you get a button press 2 or 3 go to 3
            Serial.println("transitioning from 2 to 3");
            B2State =0; //reset
            B3State = 0;
            displayFlag =1; //show 
            nextState = 3; //progress to 3
            startSecond = clock.getSecond();//set this for timeout
            digitalWrite(LCD_ENABLE,HIGH);
            delay(1000);//give time for the clock to change and LCD to power on
            }
  }

if(state == 3){
  //displayFlag =1; //show 
   //clock.getA1Time(alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
        if ((clock.getHour(h12Flag, pmFlag) == alarmHour) && (clock.getMinute() == alarmMinute) )    {
              Serial.print(" A1!");
              infiniteShockDeathSpiral();
        }


  
   delay(20);

   /*
   Serial.println("the check is here");
   Serial.print("the startsecond is ");
   Serial.println(startSecond);
   Serial.println("the clocksecond is");
   Serial.println(clock.getSecond());
   */
   if(clock.getSecond() == startSecond){ //leave after a minute
    Serial.println(startSecond);
    Serial.println(clock.getSecond());
    nextState = 1;
    digitalWrite(LCD_ENABLE,LOW);
    delay(500);//give time for the clock to change and LCD to power on
     Serial.println("going back to 1 from 3");
     displayFlag = 1;
    }else{
      nextState = 3;
       //    Serial.println("shouldn't go back");
      }

     if(B2State){ //increment hour
      B2State = 0;
      alarmHour = alarmHour +1;
      alarmHour %= 24;
      Serial.println("increment hour here");
      Serial.println(alarmHour);
      }

     if(B3State){ //increment minute
      B3State =0;
      alarmMinute = alarmMinute + 1;
      alarmMinute %= 60;
      Serial.println("increment minute here");
      Serial.println(alarmMinute);
      }

  
    //state 3 here
  //call time display routine
 
  //check for button press. if button has been pressed, then display new alarm time
  //check if time has passed
  //set nextstate
  Serial.println(" calling display from 3");
  displayTime();
  }
  

//Serial.println(Hour);
//Serial.println(Minute);
//Serial.println(clock.getHour(h12Flag, pmFlag));
//Serial.println(clock.getMinute());
//Serial.println(clock.getSecond());


oldTimeHour = Hour;
oldTimeMinute = Minute;
Hour = clock.getHour(h12Flag, pmFlag);
Minute = clock.getMinute();
Second = clock.getSecond();
if((oldTimeHour != Hour) ||oldTimeMinute !=Minute){
  displayFlag = 1;
  Serial.println("Setting displayFlag at end");
  }

state = nextState;
//Serial.println(state);
}
