#include <SPI.h>
#include "RF24.h"
#include "DHT.h"
#include <MQ2.h>
#define DHTTYPE DHT11
#define DHTPIN A0
#define MQ02 A4

DHT dht(DHTPIN, DHTTYPE);
MQ2 mq(MQ02, false, 1000);

const uint64_t pipe = 0xE8E8F0F0E1LL; // địa chỉ để phát
const uint64_t pipe1 = 0xE8E8F0F0E2A; // địa chỉ để thu

RF24 radio(9,10); //thay 10 thành 53 với mega
int text[3];
void setup(){
  //============================================================Module NRF24
  Serial.begin(9600);
  dht.begin();
  mq.begin();
  pinMode(MQ02,INPUT);
  radio.begin();
  radio.setAutoAck(1);              
  radio.setRetries(1,1);            
  radio.setDataRate(RF24_1MBPS);    // Tốc độ truyền
  radio.setPALevel(RF24_PA_MAX);      // Dung lượng tối đa
  radio.setChannel(10);               // Đặt kênh
  radio.openWritingPipe(pipe);        // mở kênh
  radio.openReadingPipe(1, pipe1); 
}

void loop(){
  int sensor[3]; 
  sensor[0] = mq.readGasConcentration();
  sensor[1] = dht.readHumidity();
  sensor[2] = dht.readTemperature();
  radio.write(&sensor, sizeof(sensor));
  Serial.println(sensor[0]);
  delay(50);
  while (sensor[0]>50) {
    radio.startListening();
    radio.read(&text, sizeof(text));
    Serial.println(text[1]);
    break;
  }
  radio.stopListening();
}
