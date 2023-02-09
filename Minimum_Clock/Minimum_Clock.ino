
//LCD init
int const DISPLAY_INTERVAL = 1000;           // interval at which to blink (milliseconds)

enum displayModes {
  NORMAL_DISPLAY, // regular display mode
  SET_ALARM, // alarm set mode
};

displayModes mode = NORMAL_DISPLAY;

#include <TFT.h>  // Arduino LCD library
#include <SPI.h>


// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8

// pin definition for the Leonardo
// #define cs   7
// #define dc   0
// #define rst  1

// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);
char hourPrintout[4];
char minutePrintout[4];
char alarmHourPrintout[4];
char alarmMinutePrintout[4];
char voltagePrintoutInteger[4];
char voltagePrintoutDecimal[4];
bool alarm1Status;

int oldHourPrintout;
int oldMinutePrintout;
int oldAlarmHourPrintout;
int oldAlarmMinutePrintout;
int oldVoltagePrintout;
bool oldAlarm1Status = true;






// char array to print to the screen

//end LCD init

//start RTC init

#include <DS3231.h>
#include <Wire.h>

DS3231 clock;
bool century = false;
bool h12Flag;
bool pmFlag;
byte alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits;
bool alarmDy, alarmH12Flag, alarmPmFlag;

//end RTC init

//LED init
#define GREEN_LED 2

#define RED_LED 4

//Shocker init
#define SHOCK 3

//battery monitoring
#define BATTERY_IN A7


//Button init
#define GREEN_BUTTON 5
#define BLUE_BUTTON 6
#define BLACK_BUTTON 7
int greenState = 0;         // variable for reading the pushbutton status
int blueState = 0;         // variable for reading the pushbutton status
bool blackState = 0;         // variable for reading the pushbutton status


void setup() {
  // put your setup code here, to run once:


    


//RTC setup
  // Start the I2C interface
  Wire.begin();
 
  // Start the serial interface
  Serial.begin(9600);
  Serial.println("test serial");

   
    clock.setClockMode(false);  // set to 24h
    //setClockMode(true); // set to 12h

  //  clock.setYear(1974);
   // clock.setMonth(12);
   // clock.setDate(23);
   // clock.setDoW(6);
   // clock.setHour(15);
   // clock.setMinute(18);
   // clock.setSecond(10);

    //set alarm
      alarmDay = clock.getDoW();
      alarmHour = clock.getHour(h12Flag, pmFlag);
      alarmMinute = clock.getMinute() +1;
      alarmSecond = 0 ;
      clock.setA1Time(clock.getDoW(), alarmHour, alarmMinute, alarmSecond, 0x0, true,  //DOW, Hour, Minute, Second, 
      false, false);
      clock.turnOnAlarm(1);
      alarm1Status = true;
   
//end RTC setup

//LCD setup


  // Put this line at the beginning of every sketch that uses the GLCD:
  TFTscreen.begin();

  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);

  // write the static text to the screen
  // set the font color to white
  TFTscreen.stroke(255, 255, 255);
  // set the font size
  TFTscreen.setTextSize(2);
  // write the text to the top left corner of the screen
  //TFTscreen.text("Time :\n ", 0, 0);
  


  // ste the font size very large for the loop
 

  
  // set the font color
  // wait for a moment

//Button Setup
 pinMode(BLACK_BUTTON, INPUT);
 pinMode(GREEN_BUTTON, INPUT);
 pinMode(BLUE_BUTTON, INPUT);

//LED indicator setup
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

//Battery monitoring Setup


//shocker setup
  pinMode(SHOCK, OUTPUT);


}


void infiniteShockDeathSpiral(){


  Serial.println("Enters ISDS function");

    while(!blackState){
         Serial.println("in shock state");
         digitalWrite(SHOCK,HIGH);
         digitalWrite(RED_LED,HIGH);
         delay(100);


         int shockDelay = 1;         
         while(shockDelay > 0 && !blackState ){
         Serial.println("lull state");
         digitalWrite(SHOCK,LOW);
         digitalWrite(RED_LED,LOW);
         delay(1000);
         blackState = digitalRead(BLACK_BUTTON);
         shockDelay--;
            }
          }
      
      blackState = false;
      Serial.println("end shock state");
  }


//occurs every 1 second
unsigned long previousMillis = 0;        // will store last time LED was updated



/*
char hourPrintout[4];
char minutePrintout[4];
char alarmHourPrintout[4];
char alarmMinutePrintout[4];
char voltagePrintout[4];

char oldHourPrintout[4];
char oldMinutePrintout[4];
char oldAlarmHourPrintout[4];
char oldAlarmMinutePrintout[4];
char oldVoltagePrintout[4];
*/


void displayAlarmTime(){
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
      Serial.println(" PM ");
    } else {
      Serial.println(" AM ");
    }
  } else {
    Serial.println(" 24h ");
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
  
  // Display Alarm 1 information
  Serial.print("Alarm 1: ");
  clock.getA1Time(alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
  Serial.print(alarmDay, DEC);
  if (alarmDy) {
    Serial.print(" DoW");
  } else {
    Serial.print(" Date");
  }
  Serial.print(' ');
  Serial.print(alarmHour, DEC);
  Serial.print(' ');
  Serial.print(alarmMinute, DEC);
  Serial.print(' ');
  Serial.print(alarmSecond, DEC);
  Serial.print(' ');
  if (alarmH12Flag) {
    if (alarmPmFlag) {
      Serial.print("pm ");
    } else {
      Serial.print("am ");
    }
  }
  if (clock.checkAlarmEnabled(1)) {
    Serial.print("enabled");
  }
  

  }


void updateLCD(){

 


    
    //serial.println


  
    unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= DISPLAY_INTERVAL) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    //do stuff here
          displayAlarmTime();
       
          String hourVal = String(clock.getHour(h12Flag, pmFlag));
          hourVal.toCharArray(hourPrintout, 4);
          
          String minuteVal = String(clock.getMinute());
          minuteVal.toCharArray(minutePrintout, 4);

          clock.getA1Time(alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag); //gets current alarm values from chip

          String alarmHourVal = String(alarmHour);
          alarmHourVal.toCharArray(alarmHourPrintout, 4);

          String alarmMinuteVal = String(alarmMinute);
          alarmMinuteVal.toCharArray(alarmMinutePrintout, 4);

          
          
          //clear screen if necessary
          if(oldMinutePrintout != clock.getMinute() || oldAlarmMinutePrintout != alarmMinute || oldAlarmHourPrintout != alarmHour || oldAlarm1Status != alarm1Status){
          TFTscreen.background(0, 0, 0);
          oldMinutePrintout = clock.getMinute(); //set new old printout
          oldAlarmMinutePrintout = alarmMinute;
          oldAlarmHourPrintout = alarmHour;
          oldAlarm1Status = alarm1Status;
          
          unsigned long rawVoltageData = 512;//analogRead(BATTERY_IN);
          //unsigned long realVoltage = 5*3*((100*rawVoltageData)/1024);
          unsigned long realVoltage = 15*(rawVoltageData*100)/1024;
          //Serial.println(realVoltage);
          
          String voltageValInteger = String(realVoltage/100); //435 becomes 4
          String voltageValDecimal = String(realVoltage - ((realVoltage/100)*100)); //435 becomes 35
                 voltageValInteger.toCharArray(voltagePrintoutInteger, 4);    
                 voltageValDecimal.toCharArray(voltagePrintoutDecimal, 4);   
                      
          }


          
          //normal display here
         // Serial.println("start screen printout ");
          //clear screen
         
        
        

          //actual display
          
          TFTscreen.stroke(255, 255, 255); //white
          
          TFTscreen.text("Time:\n ", 0, 0);
          TFTscreen.text(hourPrintout, 70, 0);
           TFTscreen.text(":", 100, 0);
          TFTscreen.text(minutePrintout, 115, 0);

          
          TFTscreen.text("Alarm: \n ", 0, 40);

          if(clock.checkAlarmEnabled(1)){
              TFTscreen.text(alarmHourPrintout, 75, 40);
          TFTscreen.text(":", 100, 40);
          TFTscreen.text(alarmMinutePrintout, 115, 40);
            }else{
              TFTscreen.text("OFF", 75, 40);
              
              }
        

          TFTscreen.text("Volts: \n ", 0, 80);
          TFTscreen.text(voltagePrintoutInteger, 75, 80);
          TFTscreen.text(".", 95, 80);
          TFTscreen.text(voltagePrintoutDecimal, 110, 80);
          
       
       
    
  }
  }


void loop() {
  // put your main code here, to run repeatedly:
  
       

     //update LCD
      updateLCD(); //handles both alarm and regular display

     /*
     //button polling
     if(digitalRead(BLUE_BUTTON)){ //increment hour
      
      alarmHour++;
      alarmHour %= 24;
      clock.setA1Time(clock.getDoW(), alarmHour, alarmMinute, alarmSecond, 0x0, true,  //DOW, Hour, Minute, Second, 
      false, false);
      delay(300);
     
     }
     
     if(digitalRead(GREEN_BUTTON)){ //increment minute
     alarmMinute++;
     alarmMinute %= 60;
   clock.setA1Time(clock.getDoW(), alarmHour, alarmMinute, alarmSecond, 0x0, true,  //DOW, Hour, Minute, Second, 
      false, false);
      delay(300);
     }
     
     if(digitalRead(BLACK_BUTTON)){//toggle alarm,
      if(clock.checkAlarmEnabled(1)){
        clock.turnOffAlarm(1);
        Serial.println("turnOffAlarm");
        alarm1Status = false;
        }else{
         clock.setA1Time(clock.getDoW(), alarmHour, alarmMinute, alarmSecond, 0x0, true,  //DOW, Hour, Minute, Second, 
        false, false);
         clock.turnOnAlarm(1);
         Serial.println("turnOnAlarm");
         alarm1Status = true;
          }
         delay(300);
     }
     */
     

//actuate
   //Serial.println(alarm1Status);
    if(clock.checkAlarmEnabled(1)){
     // Serial.println("alarm 1 enabled");
      }else{
       //     Serial.println("alarm 1 disabled");
        }
   
  if (clock.checkIfAlarm(1)) {
        Serial.print(" A1!");
       

        //runs 30 times, can't be stopped except reset button
        for(int i =0; i <30;i++){
           infiniteShockDeathSpiral();
          }
        
        digitalWrite(GREEN_LED,HIGH);
        delay(500);
        digitalWrite(GREEN_LED,LOW);
        clock.setA1Time(clock.getDoW(), alarmHour, alarmMinute, alarmSecond, 0x0, true,  //DOW, Hour, Minute, Second, 
        false, false);
        clock.turnOnAlarm(1);
        alarm1Status = true;
        
  }
}
