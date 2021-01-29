#include <SPI.h>
#include <LoRa.h>
//fce declaration
void LoRa_rxMode();
void onReceive(int packetSize);
void onTxDone();

int counter = 0;
#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);    
  digitalWrite(LED_BUILTIN, LOW);
  SerialUSB.begin(9600);
  while (!Serial);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);    
  digitalWrite(LED_BUILTIN, LOW);
  SerialUSB.println("LoRa Server non-blocking");
  
  LoRa.setPins(PA4, PB1, PB0);
  if (!LoRa.begin(433E6)) {
    SerialUSB.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSpreadingFactor(11);
  digitalWrite(LED_BUILTIN, HIGH);

  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  LoRa_rxMode();
}

void loop() {
  delay(5000); 
}

void LoRa_rxMode(){
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
}

void LoRa_sendMessage(String message) {
  LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  LoRa.print(message);                  // add payload
  LoRa.endPacket(true);                 // finish packet and send it
}

void onReceive(int packetSize) {
  String message = "";

  while (LoRa.available()) {
    message += (char)LoRa.read();
  }
  SerialUSB.println(message);
}

void onTxDone() {
  LoRa_rxMode();
}