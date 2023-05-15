#include <Wire.h>

#include "DHT.h"

#define DHTPIN 4
#define gas 5
#define flame 6

#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Wire.begin(8);                /* join i2c bus with address 8 */
  Wire.onReceive(receiveEvent); /* register receive event */
  Wire.onRequest(requestEvent); /* register request event */
  Serial.begin(9600);
  pinMode(gas, INPUT);
  pinMode(flame, INPUT); 
  Serial.println(F("DHTxx test!"));
  dht.begin();
}

void loop() {
  if(digitalRead(gas) == 1 && digitalRead(flame) == 0){
    tone(7, 400);
  } else if(digitalRead(gas) == 0 && digitalRead(flame) == 1){
    tone(8, 600);
  } else if(digitalRead(flame) == 1 && digitalRead(gas) == 1){
    tone(8, 600);
    delay(1000);
    noTone(8);
    tone(7, 400);
    delay(1000);
    noTone(7);
  } else if(digitalRead(gas) == 0 && digitalRead(flame) == 0){
    noTone(7);
    noTone(8);
  }

  // Serial.println(dht.readHumidity());
  // Serial.println(dht.readTemperature());
  // Serial.println(dht.computeHeatIndex(false));
  // Serial.println(digitalRead(gas));
  // Serial.println(digitalRead(flame));
}

// function that executes whenever data is received from master
void receiveEvent(int howMany) {
 while (0 <Wire.available()) {
    char c = Wire.read();      /* receive byte as a character */
    Serial.print(c);           /* print the character */
  }
 Serial.println();             /* to newline */
}

// function that executes whenever data is requested from master
void requestEvent(){
    Wire.write(int(dht.readHumidity()));
    Wire.write(int(dht.readTemperature()));
    Wire.write(int(dht.computeHeatIndex(false)));
    Wire.write(digitalRead(gas));
    Wire.write(digitalRead(flame));
}
