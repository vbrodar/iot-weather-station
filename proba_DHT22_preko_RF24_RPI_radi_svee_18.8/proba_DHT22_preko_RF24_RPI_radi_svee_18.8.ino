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

float rpiHumidity;
float rpiTemperature;

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
    radio.read(&data, sizeof(MyData));
    }
}
void loop()
{
  recvData();
  rpiHumidity = dht.readHumidity();
  rpiTemperature = dht.readTemperature();

  Serial.print("| Room 1 -- Arduino |\n");
  Serial.print("Humidity: ");
  Serial.print(data.h);
  Serial.print(" % ");
  lcd.setCursor(0,0);
  lcd.print("Humidity: "); 
  lcd.print(rpiHumidity);
  lcd.print("% ");
  lcd.setCursor(0,1);
  Serial.print(" Temperature: ");
  Serial.print(data.t);
  Serial.print(" °C \n");
  lcd.print("Temp. : ");
  lcd.print(rpiTemperature);
  lcd.print(" C ");

  Serial.print("| Main Room -- RPI | \n");
  Serial.print(" Humidity: ");
  Serial.print(rpiHumidity);
  Serial.print(" % ");
  Serial.print(" Temperature: ");
  Serial.print(rpiTemperature);
  Serial.print(" °C \n");
  
  delay(10000); 
  //Serial.print("\n");

    
}
