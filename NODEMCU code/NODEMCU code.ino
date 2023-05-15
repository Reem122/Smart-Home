// ip: 192.168.0.106
/*
This example shows how to connect to Cayenne using an ESP8266 and send/receive sample data.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. If you have not already installed the ESP8266 Board Package install it using the instructions here: https://github.com/esp8266/Arduino#installing-with-boards-manager.
2. Select your ESP8266 board from the Tools menu.
3. Set the Cayenne authentication info to match the authentication info from the Dashboard.
4. Set the network name and password.
5. Compile and upload the sketch.
6. A temporary widget will be automatically generated in the Cayenne Dashboard. To make the widget permanent click the plus sign on the widget.
*/

#include <Wire.h>
#include <Servo.h>
#include <ESP8266WiFi.h>

#define ldr A0
int data[6];
Servo myservo;
//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

// WiFi network info.
const char* ssid = "Your WiFi ssid";
const char* wifiPassword = "Your Wifi password";

WiFiServer server(80);

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "username";
char password[] = "password";
char clientID[] = "ID";

void setup() {
	Serial.begin(9600);
  Wire.begin(D1, D2); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(ldr, INPUT);
  myservo.attach(D4, 500, 2400);
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("..");
  }
  Serial.println("connected");
  Serial.println(WiFi.localIP());

  server.begin();
  delay(1000);
}

void loop() {
  Wire.beginTransmission(8); /* begin with device address 8 */
  Wire.write("Hello Arduino");  /* sends hello string */
  Wire.endTransmission();    /* stop transmitting */
  Wire.requestFrom(8, 5); /* request & read data of size 13 from slave */
  while(Wire.available()){
    // int data = Wire.read();
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = analogRead(ldr);

    Serial.println(data[0]);
    Serial.println(data[1]);
    Serial.println(data[2]);
    Serial.println(data[3]);
    Serial.println(data[4]);
    Serial.println(data[5]);
  }
	Cayenne.loop();
  Cayenne.virtualWrite(0, data[0], TYPE_RELATIVE_HUMIDITY, UNIT_PERCENT);
  Cayenne.virtualWrite(1, data[1], TYPE_TEMPERATURE, UNIT_CELSIUS);
  Cayenne.virtualWrite(2, data[2], TYPE_TEMPERATURE, UNIT_CELSIUS);
  Cayenne.virtualWrite(3, data[3]==1?0:1, TYPE_DIGITAL_SENSOR, UNIT_DIGITAL);
  Cayenne.virtualWrite(4, data[4]==1?0:1, TYPE_DIGITAL_SENSOR, UNIT_DIGITAL);
  Cayenne.virtualWrite(5, data[5], TYPE_LUMINOSITY, UNIT_RATIO);

  WiFiClient client;
  client = server.available();

  if(client == 1){
    String request = client.readStringUntil('\n');
    client.flush();
    Serial.println(request);
    if(request.indexOf("led1on") != -1){
      digitalWrite(D5, HIGH);      
    }
    else if(request.indexOf("led2on") != -1){
      digitalWrite(D6, HIGH);
    }
    else if(request.indexOf("led3on") != -1){
      digitalWrite(D7, HIGH);
    }
    else if(request.indexOf("ledallon") != -1){
      digitalWrite(D5, HIGH); 
      digitalWrite(D6, HIGH);
      digitalWrite(D7, HIGH);
    }
    else if(request.indexOf("led1off") != -1){
      digitalWrite(D5, LOW);
    }
    else if(request.indexOf("led2off") != -1){
      digitalWrite(D6, LOW);
    }
    else if(request.indexOf("led3off") != -1){
      digitalWrite(D7, LOW);
    }
    else if(request.indexOf("ledalloff") != -1){
      digitalWrite(D5, LOW); 
      digitalWrite(D6, LOW);
      digitalWrite(D7, LOW);
    }
    else if(request.indexOf("opendoor") != -1){
      for(int i=0; i<=180; i++){
        myservo.write(i);
        delay(15);
      }
    }
    else if(request.indexOf("closedoor") != -1){
        for(int i=180; i>=0; i--){
        myservo.write(i);
        delay(15);
      }  
    }
  }
}

CAYENNE_IN(6)
{
	int value = getValue.asInt();
  if (value == 1){
	  analogWrite(D5, 255);
  } else{
    analogWrite(D5, 0);
  }
}
CAYENNE_IN(7)
{
	int value = getValue.asInt();
	if (value == 1){
	  analogWrite(D6, 255);
  } else{
    analogWrite(D6, 0);
  }
}
CAYENNE_IN(8)
{
	int value = getValue.asInt();
	if (value == 1){
	  analogWrite(D7, 255);
  } else{
    analogWrite(D7, 0);
  }
}
CAYENNE_IN(9)
{
	int value = getValue.asInt();
	analogWrite(D5, value);
}
CAYENNE_IN(10)
{
	int value = getValue.asInt();
	analogWrite(D6, value);
}
CAYENNE_IN(11)
{
	int value = getValue.asInt();
	analogWrite(D7, value);
}
CAYENNE_IN(13)
{
	int value = getValue.asInt();
  if(value == 0){
    for(int i=180; i>=0; i--){
      myservo.write(i);
      delay(15);
    }
  } else{
    for(int i=0; i<=180; i++){
      myservo.write(i);
      delay(15);
    }
  }
  Cayenne.virtualWrite(14, value, TYPE_DIGITAL_SENSOR, UNIT_DIGITAL);
}
CAYENNE_IN(20)
{
  int value = getValue.asInt();
  if (value == 1){
    int light = map(analogRead(ldr), 0, 1023, 0, 255);
    analogWrite(D5, 255-light);
    analogWrite(D6, 255-light);
    analogWrite(D7, 255-light);
  }
}