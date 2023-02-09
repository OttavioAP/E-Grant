#include <TFT.h>  // Arduino LCD library
#include <SPI.h>
#include <DS3231.h>
#include <Wire.h>


// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8



//start RTC init


DS3231 clock;
bool century = false;
bool h12Flag;
bool pmFlag;
byte alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits;
bool alarmDy, alarmH12Flag, alarmPmFlag;
//end RTC init

#define RED_LED 2
//Shocker init
#define SHOCK 3


void setup() {
  // put your setup code here, to run once:

//RTC setup
  // Start the I2C interface
  pinMode(12, OUTPUT);
  digitalWrite(12,HIGH);//turn on the RTC chip
  Wire.begin();
 
  // Start the serial interface
  Serial.begin(9600);
  Serial.println("test serial");

   /* 
    // time reset stuff
    clock.setClockMode(false);  // set to 24h
    //setClockMode(true); // set to 12h

    clock.setYear(1975);
    clock.setMonth(1);
   clock.setDate(31);
    clock.setDoW(3);
    clock.setHour(20);
    clock.setMinute(40);
    clock.setSecond(30);
*/

    //set alarm
      alarmDay = clock.getDoW();
      //alarmHour = clock.getHour(h12Flag, pmFlag)+2;
      alarmHour = 13; //oot
      alarmMinute = 0;
      //alarmMinute = clock.getMinute();
      alarmSecond = 0 ;
      clock.setA1Time(clock.getDoW(), alarmHour, alarmMinute, alarmSecond, 0x0, true,  //DOW, Hour, Minute, Second, 
      false, false);
      clock.turnOnAlarm(1);
  
//end RTC setup


//shocker setup
  pinMode(SHOCK, OUTPUT);

}

void infiniteShockDeathSpiral(){


  Serial.println("Enters ISDS function");

    while(true){
         Serial.println("in shock state");
         analogWrite(SHOCK,003);
         digitalWrite(RED_LED,HIGH);
         delay(100);

         Serial.println("lull state");
         digitalWrite(SHOCK,LOW);
         digitalWrite(RED_LED,LOW);
         delay(3000);
          
          }
      
  }



void loop() {
  // put your main code here, to run repeatedly:
   clock.getA1Time(alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
  if (clock.checkIfAlarm(1) ||((clock.getHour(h12Flag, pmFlag) == alarmHour) && (clock.getMinute() == alarmMinute) )    ){
        Serial.print(" A1!");
        infiniteShockDeathSpiral();
  }
}
