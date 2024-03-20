#include <esp_now.h>
#include <WiFi.h>

#include <Wire.h>
#include "Adafruit_DRV2605.h"

#define max_FSR_Value 1300
#define min_force_threshold 150 

Adafruit_DRV2605 drv;
// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    int a;
    int b;
    int c;
    int d;
} struct_message;

// Create a struct_message called myData
struct_message myData;

void fourPrints(int a, int b, int c, int d){
  Serial.print(a);
  Serial.print(" ");
  Serial.print(b);
  Serial.print(" ");
  Serial.print(c);
  Serial.print(" ");
  Serial.print(d);
  Serial.print(" ");
  Serial.println();
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  // Serial.print("Bytes received: ");
  // Serial.println(len);
  // Serial.print("BALL: ");
  // Serial.println(myData.a);
  // Serial.print("HEEL: ");
  // Serial.println(myData.b);
  // Serial.print("TOE: ");
  // Serial.println(myData.c);
  // Serial.print("SIDE: ");
  // Serial.println(myData.d);
  // Serial.println();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

   if (! drv.begin()) {
    Serial.println("Could not find DRV2605");
    while (1) delay(10);
  }
  drv.setMode(DRV2605_MODE_PWMANALOG); 

  pinMode(33,OUTPUT);
  pinMode(32,OUTPUT);
  pinMode(26,OUTPUT);
  pinMode(25,OUTPUT);
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  int tact1 = 128 + ((myData.a-500)*128)/max_FSR_Value;
  int tact2 = 128 + ((myData.b-500)*128)/max_FSR_Value;
  int tact3 = 128 + ((myData.c-500)*128)/max_FSR_Value;
  int tact4 = 128 + ((myData.d-500)*128)/max_FSR_Value;
  fourPrints(myData.a, myData.b, myData.c, myData.d);
  int pwm1 = (tact1>min_force_threshold)?255:128;
  int pwm2 = (tact2>min_force_threshold)?255:128;
  int pwm3 = (tact3>min_force_threshold)?255:128;
  int pwm4 = (tact4>min_force_threshold)?255:128;
  fourPrints(pwm1, pwm2, pwm3, pwm4);
  analogWrite(33, pwm1); //ball
  analogWrite(32, pwm3); //heel
  analogWrite(26, pwm2); //toe
  analogWrite(25, pwm4); //side
  delay(200);
  // Serial.println(WiFi.macAddress());

  // int i;
  // for(i=128; i<256; i++){
  //   analogWrite(33, i);
  //   delay(20);
  // }
  // analogWrite(33,128);
  // analogWrite(32, 128);
  // analogWrite(26, 128);
  // analogWrite(25, 128);
  // delay(100);
  // for(i=128; i<256; i++){
  //   analogWrite(32, i);
  //   delay(20);
  // }
  // analogWrite(32,128);
  // delay(100);
  // for(i=128; i<256; i++){
  //   analogWrite(26, i);
  //   delay(20);
  // }
  // analogWrite(26,128);
  // delay(100);
  // for(i=128; i<256; i++){
  //   analogWrite(25, i);
  //   delay(20);
  // }
  // analogWrite(25,128);
  // delay(100);
}