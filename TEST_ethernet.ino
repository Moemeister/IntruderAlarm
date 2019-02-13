/********************************
 * Libraries included
 *******************************/
#include <Ethernet.h>
#include <SPI.h>
#include <UbidotsEthernet.h>
#include <SoftwareSerial.h>
int trigPin = 2;
int echoPin = 4;
long duration, cm, inches;

/********************************
 * Constants and objects
 *******************************/
/* Assigns the Ubidots parameters */
char const * TOKEN = "BBFF-fbDRNk98dK7Mat06NmdFmkeepnpDyA"; // Assign your Ubidots TOKEN
char const * VARIABLE_LABEL_1 = "Distancia"; // Assign the unique variable label to send the data

/* Enter a MAC address for your controller below */
/* Newer Ethernet shields have a MAC address printed on a sticker on the shield */
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

/* initialize the instance */
Ubidots client(TOKEN);

/********************************
 * Main Functions
 *******************************/
void setup() {
  Serial.begin(9600);
  //client.setDebug(true);// uncomment this line to visualize the debug message
  /* start the Ethernet connection */
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.print(F("Starting ethernet..."));
  if (!Ethernet.begin(mac)) {
    Serial.println(F("failed"));
  } else {
    Serial.println(Ethernet.localIP());
  }
  /* Give the Ethernet shield a second to initialize */
  delay(1000);
  Serial.println(F("Ready"));
}

void loop() {

  Ethernet.maintain();
  digitalWrite(trigPin, LOW);
  delayMicroseconds(1);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  inches = (duration / 2) / 74;
  Serial.print("Distancia: ");
  Serial.println(inches);
  if (inches < 20 || inches > 500) {
    Serial.println("Intruder Detected!");
    Serial.println("Sending text Notification...");
    delay(5000);
    Serial.println("Patrol Mode Initiated...");
  }
  /* Sensors readings */

  /* Sending values to Ubidots */
  client.add(VARIABLE_LABEL_1, inches);
  
  client.sendAll();
  //delay(300);
}
