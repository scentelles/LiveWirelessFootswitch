
#include <ESP8266WiFi.h>         // https://github.com/esp8266/Arduino

char ssid[] = "slyzic-hotspot";                   // your network SSID (name)
char pass[] = "totototo";       // your network password


#include "OSCManager.h"

// Channel Voice Messages
#define MIDI_STATUS_NOTE_OFF 0x80
#define MIDI_STATUS_NOTE_ON 0x90
#define MIDI_STATUS_CONTROL_CHANGE 0xb0
#define MIDI_STATUS_PROGRAM_CHANGE 0xc0
#define MIDI_STATUS_COMMAND_CONTROL_CHANGE 0xb0
#define MIDI_STATUS_CONTROL_CHANGE_FROM_GUI 0x21

#define PIN_SWITCH1 D3
#define PIN_SWITCH2 D4


OSCManager * myOSCManager_;

byte commandByte;
byte value1;

int switch1Value = 0;
int switch2Value = 0;
int switch3Value = 0;

int lastDebounceTime = 0;




void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  delay(500);

  pinMode(D4, OUTPUT);
  digitalWrite(D4, HIGH);




  //TODO : add Wifi timeout
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    digitalWrite(D4, HIGH);
    delay(100);
    digitalWrite(D4, LOW);
    Serial.print(".");
  }
  Serial.println("Connected to Wifi");

  myOSCManager_ = new OSCManager(new IPAddress(10, 3, 141, 1), 8000, 8887);
  myOSCManager_->setup();


  pinMode(PIN_SWITCH1, INPUT);
  pinMode(PIN_SWITCH2, INPUT);
}



void checkInputPins()
{
  int pin1Value = digitalRead(PIN_SWITCH1);
  int pin2Value = digitalRead(PIN_SWITCH2);

  switch1Value = 0;
  switch2Value = 0;
  switch3Value = 0;

  if (pin1Value == 0 and pin2Value == 0)
  {
    switch3Value = 1;
  }
  else if (pin1Value == 0)
  {
    switch1Value = 1;
  }
  else if (pin2Value == 0)
  {
    switch2Value = 1;
  }


}

void checkValueAndSend(int switchId, int switchValue)
{
  if (switchValue != 0)
  {
    if (millis() - lastDebounceTime > 1000) {
      Serial.print("Sending Command thru OSC for Switch : ");
      Serial.println(switchId);
      myOSCManager_->sendOSCMessage("/switch", switchId, switchValue);
      lastDebounceTime = millis();
      digitalWrite(D4, LOW); //turn on leds
    }
  }
}

void loop() {
  checkInputPins();

  checkValueAndSend(1, switch1Value);
  checkValueAndSend(2, switch2Value);
  checkValueAndSend(3, switch3Value);
  delay(10);
  digitalWrite(D4, HIGH); //turn led off
}
