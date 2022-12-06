/*
  LoRa Duplex communication
  Sends a message every half second, and polls continually
  for new incoming messages. Implements a one-byte addressing scheme,
  with 0xFF as the broadcast address.
  Uses readString() from Stream class to read payload. The Stream class'
  timeout may affect other functuons, like the radio's callback. For an
  created 28 April 2017
  by Tom Igoe
*/
#include <SPI.h>              // include libraries
#include <LoRa.h>

#define RADIO_CS_PIN 5
#define RADIO_DI0_PIN 2
#define RADIO_RST_PIN 14

#define BUZZER_PIN 21 // ESP32 GIOP21 pin connected to Buzzer's pin


String outgoing;              // outgoing message

byte msgCount = 0;            // count of outgoing messages
byte localAddress = 0xFF;     // address of this device
//byte destination = 0xFF;      // destination to send to
long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends
int i;
void setup() {
  Serial.begin(9600);                   // initialize serial
  pinMode(BUZZER_PIN, OUTPUT); 
  ledcSetup(0,1E5,12);
  ledcAttachPin(27,0);
  
  while (!Serial);

  Serial.println("LoRa Duplex");

  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DI0_PIN);

  if (!LoRa.begin(915E6)) {             // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }

  Serial.println("LoRa init succeeded.");
}

void loop() {
 
  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
}

 void uvsound()
  {
  ledcAttachPin(27,0);
  for(i=0;i<=4;i++){
  ledcWriteTone(0,700);
  delay(800);
  uint8_t octave = 1;
  ledcWriteTone(0,900);
  //ledcWriteNote(0,NOTE_A,octave);  
  delay(800);}
  pinMode(27,LOW);
  return;
  }
  void pir()
  {
  ledcAttachPin(27,0);
  for(i=0;i<=4;i++){
  ledcWriteTone(0,800);
  delay(1000);
  uint8_t octave = 1;
  ledcWriteTone(0,900);
  ledcWriteNote(0,NOTE_C,octave);  
  delay(1000);}
  pinMode(27,LOW);
  return;
  }
  
void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  
  
  if (incomingLength != incoming.length()) {   // check length for error
    Serial.println("error: message length does not match length");
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    Serial.println("This message is not for me.");
    return;                             // skip rest of function
  }

  // if message is for this device, or broadcast, print details:
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();
  
 // String myString = "ArduinoGetStarted.com";
  String sub = incoming.substring(0,1);
  Serial.println(sub);
  
 // String myString = "ArduinoGetStarted.com";
  String sub1 = incoming.substring(1,2);
  Serial.println(sub1);
  

 if (sub == "1") {
    Serial.println("Elephant Alert");
    pir();
  }
  else if (sub1 == "3") {
    Serial.println("Ultra On");
    uvsound();
  }
  
  delay(1000);
  }
