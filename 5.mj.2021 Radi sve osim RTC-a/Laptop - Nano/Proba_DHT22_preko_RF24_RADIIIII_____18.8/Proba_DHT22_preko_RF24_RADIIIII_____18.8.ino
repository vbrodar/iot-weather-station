#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT.h>
const uint64_t pipeOut = 0xE8E8F0F0E1LL; 
#define DHTPIN 3
#define DHTTYPE DHT22 
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
  dht.begin();
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipeOut);
}
void loop()
{
  data.h = dht.readHumidity();
  data.t = dht.readTemperature();
  if (isnan(data.h) || isnan(data.t)){
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
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
