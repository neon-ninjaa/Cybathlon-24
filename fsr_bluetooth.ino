#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setup() {
  pinMode(25, INPUT);
  pinMode(26, INPUT);
  pinMode(32, INPUT);
  pinMode(33, INPUT);
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
}

void loop() {
  int fsr1 = analogRead(25);
  int fsr2 = analogRead(26);
  int fsr3 = analogRead(32);
  int fsr4 = analogRead(33);
  String readings = "HEEL " + String(fsr1) + " BALL " + String(fsr2) + " SIDE " + String(fsr3) + " TOE " + String(fsr4) + '\n';
  Serial.println(readings);
  for(int i=0; i<readings.length(); i++) {
    if (i+8 > readings.length()) {
      char buf[8];
      readings.substring(i,readings.length()).toCharArray(buf, 8);
      SerialBT.write(*buf);
    }
    else {
      char buf[8];
      readings.substring(i,i+8).toCharArray(buf, 8);
      SerialBT.write(*buf);
      
    }
  }
  // Serial.println(readings);
  // Serial.print("HEEL ");
  // Serial.print(fsr1);
  // Serial.print(" BALL ");
  // Serial.print(fsr2);
  // Serial.print(" SIDE ");
  // Serial.print(fsr3);
  // Serial.print(" TOE ");
  // Serial.println(fsr4);
  delay(10);
  
  
}