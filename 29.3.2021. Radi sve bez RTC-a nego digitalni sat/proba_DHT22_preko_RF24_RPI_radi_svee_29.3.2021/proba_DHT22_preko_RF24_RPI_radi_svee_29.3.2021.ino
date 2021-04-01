#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
//Library za vrijeme
#include <Time.h>
#include <TimeLib.h>


#define DHTPIN 3 // What pin we are using
#define DHTTYPE DHT22 // DHT22
DHT dht(DHTPIN,DHTTYPE); //// Initialize DHT senzor

LiquidCrystal_I2C lcd(0x27, 16, 2);
const uint64_t pipeIn = 0xE8E8F0F0E1LL; 
RF24 radio(9, 10);

struct MyData {
  byte h; 
  byte t;
};

MyData data;

byte rpiHumidity;
byte rpiTemperature;

void setup()
{
  Serial.begin(9600);
  setTime(14,31,0,31,3,21);
  dht.begin();
  lcd.init();
  radio.begin();
  lcd.home();
  lcd.backlight();
  lcd.clear();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, pipeIn);
  radio.startListening();
  //lcd.println("Receiver ");
}

void digitalClockDisplay()
{

  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.print(year());
  Serial.println();  
}
void printDigits(int digits)
{
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);  
}
void recvData()
{
  if ( radio.available() ) {
    radio.read(&data, sizeof(MyData)); //Provjeriti status poslano, arduino postaviti na deep sleep
    }
}
void loop()
{
  recvData();
  rpiHumidity = dht.readHumidity();
  rpiTemperature = dht.readTemperature();

  Serial.print("\nTime and date: ");
  digitalClockDisplay();
  
  Serial.print("| Room 1 -- Arduino |\n");
  lcd.setCursor(0,0);
  Serial.print(" Temperature: ");
  Serial.print(data.t);
  Serial.print(" C ");
  lcd.print("Temp.: ");
  lcd.print(rpiTemperature);
  lcd.print(" C ");
   Serial.print("Humidity: ");
  Serial.print(data.h);
  Serial.print(" % \n");
  lcd.setCursor(0,1);
  lcd.print("Humidity: "); 
  lcd.print(rpiHumidity);
  lcd.print("% ");

  Serial.print("| Main Room -- RPI | \n");
  Serial.print(" Temperature: ");
  Serial.print(rpiTemperature);
  Serial.print(" C ");
  Serial.print(" Humidity: ");
  Serial.print(rpiHumidity);
  Serial.print(" % \n");
  
  delay(10000); 
  //Serial.print("\n");

    
}
