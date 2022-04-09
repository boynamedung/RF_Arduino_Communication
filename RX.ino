#include <SPI.h>
#include "RF24.h"
#include <Servo.h>

#define Fan A4
#define buzzer 2
Servo myservo;  // create servo object to control a servo
Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
const uint64_t pipe = 0xE8E8F0F0E1LL; // địa chỉ thu
RF24 radio(9,10);//(CE, CSN)

int counter=0;
int counter1=0;
int counter2=0;
int sensor[5];
int gas_pos;
boolean warning = 0;
void setup(){
  Serial.begin(9600);
  radio.begin();                    
  radio.setAutoAck(1);             
  radio.setDataRate(RF24_1MBPS);    // Tốc độ dữ liệu
  radio.setChannel(10);               // Đặt kênh
  radio.openReadingPipe(1,pipe);    
  radio.startListening();
  myservo.attach(2);  // Cua so trai
  myservo1.attach(3); // Cua so phai
  myservo2.attach(4);  // Cua so trai
  myservo3.attach(5); // Cua so phai
  pinMode(Fan,OUTPUT);
  pinMode(buzzer, OUTPUT);
}
 
void loop(){
  if (radio.available()){
    while (radio.available()){
      radio.read(&sensor, sizeof(sensor));
      Serial.print(F("Light value 1: "));
      Serial.print(sensor[0]);
      Serial.print((" lux "));
      Serial.print(F("Light value 2: "));
      Serial.print(sensor[1]);
      Serial.print((" lux "));
      Serial.print(F("Gas Level: "));
      Serial.print(sensor[2]);
      Serial.print((" ppm"));
      Serial.print(F(" Humidity: "));
      Serial.print(sensor[3]);
      Serial.print(F(" Temperature: "));
      Serial.print(sensor[4]);
      Serial.print((" Celcius degree"));
      Serial.println();
      }
      if (sensor[0] > 1000) {
        Serial.println("Phong khach bi choi");
        if (counter == 0){
          myservo2.write(0);
          delay(250);            
          myservo2.write(90); 
          delay(250);
          myservo3.write(180);
          delay(250);            
          myservo3.write(90); 
          delay(250);
          counter=1;
          }
        delay(50);
        }
      if (sensor[1] > 1000) {
        Serial.println("Phong ngu choi qua");
        if (counter1 == 0){
          myservo2.write(0);
          delay(250);            
          myservo2.write(90); 
          delay(250);
          myservo3.write(180);
          delay(250);            
          myservo3.write(90); 
          delay(250);
          counter1=1;
          }
        delay(50);
        }
      if (sensor[2] > 0) {
        gas_pos=sensor[2];
        }
        else
        gas_pos=-sensor[2];
      if (gas_pos > 1000) 
      {
        //Serial.println("Gas Alert!!!!");
        analogWrite(Fan, 675);
        Serial.println("Gas Alert!!!!");
        if (counter2 == 0) 
        {
          myservo2.write(0);
          delay(250);            
          myservo2.write(90); 
          delay(250);
          counter2 =1;
          Serial.println("Servo on");
        }
        for (int k=0;k<20;k++){
           buzzeralert(); 
        } 
       } else 
          {
          analogWrite(Fan,0);
          }
  }
}
void buzzeralert(void)
{
  unsigned char i;
    for (i = 0; i <100; i++) // When a frequency sound
    {
      digitalWrite (buzzer, HIGH) ; //send tone
      delay (1) ;
      digitalWrite (buzzer, LOW) ; //no tone
      delay (1) ;
    }
}
