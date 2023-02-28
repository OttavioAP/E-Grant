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
byte alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits,Hour,Minute,Second;
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
      alarmHour = 11 ; //oot
      alarmMinute = 42;
      //alarmMinute = clock.getMinute();
      alarmSecond = 0 ;
      clock.setA1Time(clock.getDoW(), alarmHour, alarmMinute, alarmSecond, 0x0, true,  //DOW, Hour, Minute, Second, 
      false, false);
      clock.turnOnAlarm(1);
  
//end RTC setup


//shocker setup
  pinMode(SHOCK, OUTPUT);

  //SET THIS TOO
  clock.setClockMode(false);  // set to 24h
    //setClockMode(true); // set to 12h
    Hour = 4;
    Minute = 18;
    Second = 0;

    clock.setHour(Hour);
    clock.setMinute(Minute);
    clock.setSecond(Second);



}

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



void loop() {
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
  
  // put your main code here, to run repeatedly:
   clock.getA1Time(alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
  if (clock.checkIfAlarm(1) ||((clock.getHour(h12Flag, pmFlag) == alarmHour) && (clock.getMinute() == alarmMinute) )    ){
        Serial.print(" A1!");
        infiniteShockDeathSpiral();
  }
}
