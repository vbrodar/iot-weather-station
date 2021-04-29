#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

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
  
  //Serial.print("Arduino \n");
  //Serial.print("T: ");
  Serial.print(data.t);
  //Serial.print(" C ");
  //Serial.print("H: ");
  Serial.print(data.h);
  Serial.print("\n");
  //Serial.print(" % \n");

  //Serial.print("RPI  \n");
  //Serial.print("T: ");
  Serial.print(rpiTemperature);
  //Serial.print("\n");
  //Serial.print(" C ");
  //Serial.print("H: ");
  Serial.print(rpiHumidity);
  Serial.print("\n");
  //Serial.print(" % \n");
  
  delay(10000); 
  //Serial.print("\n");

    
}
