#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

bool enableHeater = false;

Adafruit_SHT31 sht31 = Adafruit_SHT31();

int button = 7;
int blueLed = 4;
float threshold;
float h;
int s=0;

void setup() {
  Serial.begin(9600);

  while (!Serial)
    delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("SHT31 test");
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }

  //set initial threshold current humidity +10%
  h = sht31.readHumidity();
  threshold = h *1.1; 
  
  pinMode(blueLed, OUTPUT); //blueLed
  pinMode(button, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT); // red LED
}

void loop() {
  h = sht31.readHumidity();
  
  if(s==0) { //s=0 monitoringHumidity
    Serial.println("s0");
    Serial.print("Hum. % = "); Serial.println(h);
    Serial.print("Treshold. % = "); Serial.println(threshold);
    if(h>threshold) { //switch state if true
      s=1;
    } else { //keep monitoring
      digitalWrite(LED_BUILTIN, HIGH); //red Led is on
      if (digitalRead(button)==0) {      //buttonPressed?
        threshold = threshold *1.10;
        Serial.print("Treshold. % = "); Serial.println(threshold);
        delay(100);
      }
    }    
  } else if(s==1) { //s=1 thresholdCrossed
    Serial.println("s1");
    digitalWrite(blueLed, HIGH);
    digitalWrite(LED_BUILTIN, LOW);
    if(digitalRead(button)==0) { // confirm, red Led, end
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(blueLed, LOW);
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
      s=3;
    }
  } else if(s==3) {
    Serial.println("s3");
    
    }
}
