#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
// uint8_t broadcastAddress[] = {0xC8 , 0xF0, 0x9E, 0x9C, 0x13, 0xCC};
uint8_t broadcastAddress[] = {0x78 , 0x21, 0x84, 0x7F, 0x6F, 0x38};
// 78:21:84:7F:6F:38


// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int a;
  int b;
  int c;
  int d;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  // Setting up pins
  pinMode(34, INPUT);
  pinMode(35, INPUT);
  pinMode(32, INPUT);
  pinMode(33, INPUT);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  // Set values to send
  myData.a = analogRead(34);
  myData.b = analogRead(35);
  myData.c = analogRead(32);
  myData.d = analogRead(33);
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
    Serial.print("BALL: ");
    Serial.println(myData.a);
    Serial.print("HEEL: ");
    Serial.println(myData.b);
    Serial.print("SIDE: ");
    Serial.println(myData.c);
    Serial.print("TOE: ");
    Serial.println(myData.d);
    Serial.println();
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(200);
}