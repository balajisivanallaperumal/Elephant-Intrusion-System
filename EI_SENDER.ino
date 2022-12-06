
#include <SPI.h>              // include libraries
#include <LoRa.h>

#define RADIO_CS_PIN 5
#define RADIO_DI0_PIN 2
#define RADIO_RST_PIN 14


//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

const int trigPin = 32;
const int echoPin = 33;

long duration;
float distanceCm;
float distanceInch;
float Time = 0;


unsigned long previousMillis=0;
unsigned long currentMillis;


String outgoing ;              // outgoing message
String message ;
byte msgCount = 0;            // count of outgoing messages
byte localAddress = 0xAA;     // address of this device
byte destination = 0xBB;      // destination to send to
long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends

String value = "";
  

int ledPin = 26;                // choose the pin for the LED
int inputPin = 27;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0; 

void setup() {
  
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input


  Serial.begin(9600);                   // initialize serial
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

  val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
     // value = "Yes";
  
    }value = "1";
    message = value;
    
  } else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    if (pirState == HIGH) {
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }value = "2";
    message = value;
    
  }
   
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  //Serial.print("Distance (inch): ");
  //Serial.println(distanceInch);

  while(distanceCm<10){
    currentMillis = (millis()/1000);
    Serial.println("Distance<10");
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
  
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
  
    // Calculate the distance
    distanceCm = duration * SOUND_SPEED/2;
    Time = currentMillis - previousMillis;
  /*  if (Time > 20)   {
      return;
      }*/
    Serial.println(Time);
    delay(500);
  }
  String value1 = "";
  if((Time<15) && (Time>10)){
    Serial.println("Elephant is detected");
    Serial.println(Time);
    previousMillis = currentMillis;
    currentMillis = (millis()/1000);
    value1 = "3";
    message += value1;
    
    sendMessage(message);
    Serial.println("Sending " + message);
    
  }
  else{ 
    Serial.println("Elephant is not detected");
    Serial.println(Time);
    value1 = "4";
    message += value1;
    sendMessage(message);
    Serial.println("Sending " + message);
    previousMillis = currentMillis;
    currentMillis = (millis()/1000);
    delay(500);
  }
/*noInterrupts();
  timer0_millis = 0;
  interrupts ();
  delay(1000);*/
  
  Time = 0;

 //String message = value + value1;

  
 //if (millis() - lastSendTime > interval) {
    //sendMessage(message);
//Serial.println("Sending " + message);
    //lastSendTime = millis();            // timestamp the message
    //interval = random(2000) + 1000;    // 2-3 seconds
  //}
//value = "No";
}


void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}
  
