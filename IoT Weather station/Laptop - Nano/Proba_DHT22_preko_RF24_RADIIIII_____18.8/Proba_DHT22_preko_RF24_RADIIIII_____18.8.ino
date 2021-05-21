#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT.h>
#include <DS3231.h>
const uint64_t pipeOut = 0xE8E8F0F0E1LL; 
#define DHTPIN 3
#define DHTTYPE DHT22

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
  rtc.setTime(9, 10, 0);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(21, 5, 2021);   // Set the date to January 1st, 2014
 
}
void loop()
{
  
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
  delay(10000);
  radio.write(&data, sizeof(MyData));
}
