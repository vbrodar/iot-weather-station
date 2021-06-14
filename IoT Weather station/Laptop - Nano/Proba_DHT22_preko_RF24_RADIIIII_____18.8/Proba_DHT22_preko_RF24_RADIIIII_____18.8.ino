#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT.h>
#include <DS3231.h>
#include <DS3232RTC.h> //https://github.com/JChristensen/DS3232RTC  Library for alarms through RTC
#include <avr/sleep.h>//this AVR library contains the methods that controls the sleep modes

const uint64_t pipeOut = 0xE8E8F0F0E1LL; 
#define DHTPIN 3
#define DHTTYPE DHT22
#define interruptPin 2 //Pin we are going to use to wake up the Arduino

const int time_interval=1;// Sets the wakeup interval in minutes
int sum_time;
DS3231  rtc(SDA, SCL);
DHT dht(DHTPIN, DHTTYPE);


RF24 radio(9, 10); //  CN and CSN  pins of nrf
struct MyData {
  byte h;
  byte t;
};
MyData data;
void setup()
{
  pinMode(5, OUTPUT); // Pin 5 on Arduino Nano - Green LED
  
  Serial.begin(9600);
  //rtc.begin(); //beginning of rtc
  dht.begin();
  rtc.begin();
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipeOut);
  
  //Adding rtc functionality

  rtc.setDOW(FRIDAY);     // Set Day-of-Week to SUNDAY
  rtc.setTime(13, 00, 0);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(14, 6, 2021);   // Set the date to January 1st, 2014

  //initialize the alarms to known values, clear alarm flags, clear the alarm interrupt flags
  RTC.setAlarm(ALM1_MATCH_DATE, 0, 0, 1, 0);
  RTC.setAlarm(ALM2_MATCH_DATE, 0, 0, 1, 0);
  RTC.alarm(ALARM_1);
  RTC.alarm(ALARM_2);
  RTC.alarmInterrupt(ALARM_1, false);
  RTC.alarmInterrupt(ALARM_2, false);
  RTC.squareWave(SQWAVE_NONE);

   /*
     * Uncomment the block block to set the time on your RTC. Remember to comment it again 
     * otherwise you will set the time at everytime you upload the sketch
     * /
     /* Begin block
     tmElements_t tm;
    tm.Hour = 00;               // set the RTC to an arbitrary time
    tm.Minute = 00;
    tm.Second = 00;
    tm.Day = 4;
    tm.Month = 2;
    tm.Year = 2018 - 1970;      // tmElements_t.Year is the offset from 1970
    RTC.write(tm);              // set the RTC from the tm structure
     Block end * */
     time_t t; //create a temporary time variable so we can set the time and read the time from the RTC
    t=RTC.get();//Gets the current time of the RTC
    RTC.setAlarm(ALM1_MATCH_MINUTES , 0, minute(t)+time_interval, 0, 0);// Setting alarm 1 to go off every 5 minutes
    // clear the alarm flag
    RTC.alarm(ALARM_1);
    // configure the INT/SQW pin for "interrupt" operation (disable square wave output)
    RTC.squareWave(SQWAVE_NONE);
    // enable interrupt output for Alarm 1
    RTC.alarmInterrupt(ALARM_1, true);
 
}
void loop()
{
  Going_To_Sleep();
 
}

void Going_To_Sleep(){
    sleep_enable();//Enabling sleep mode
    attachInterrupt(0, wakeUp, LOW);//attaching a interrupt to pin d2
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);//Setting the sleep mode, in our case full sleep
    digitalWrite(5,LOW);//turning LED off
    time_t t;// creates temp time variable
    t=RTC.get(); //gets current time from rtc
    Serial.println("Sleep  Time: "+String(hour(t))+":"+String(minute(t))+":"+String(second(t)));//prints time stamp on serial monitor
    delay(1000); //wait a second to allow the led to be turned off before going to sleep
    sleep_cpu();//activating sleep mode
    Serial.println("Woke up!");//next line of code executed after the interrupt 
    digitalWrite(5,HIGH);//turning LED on
    data.h = dht.readHumidity();
    data.t = dht.readTemperature();
     if (isnan(data.h) || isnan(data.t)){
      Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  //Date and time 
   // Send Day-of-Week
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  
  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");
  // Send time
  Serial.println(rtc.getTimeStr());

  //temp and humidity print
  Serial.print("| Room 1 -- Arduino: | \n");
  Serial.print(" Temperature:  ");
  Serial.print(data.t);
  Serial.print(" °C ");
  Serial.print("Humidity: ");
  Serial.print(data.h);
  Serial.print(" % ");
  Serial.print("\n");
  //delay(10000);
  radio.write(&data, sizeof(MyData));
  t=RTC.get();
  //Set New Alarm
  sum_time = minute(t)+time_interval;
  if (sum_time >= 59) { //Checking if one minute has passed if it has reset seconds
  sum_time = sum_time - 60;
  }
  else {
  sum_time = sum_time;
  }
  RTC.setAlarm(ALM1_MATCH_MINUTES , 0, sum_time, 0, 0);
  // clear the alarm flag
  RTC.alarm(ALARM_1);
  }

void wakeUp(){
  Serial.println("Interrrupt Fired");//Print message to serial monitor
   sleep_disable();//Disable sleep mode
  detachInterrupt(0); //Removes the interrupt from pin 2;
 
}
