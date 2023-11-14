
#include "BluetoothSerial.h"  // include BT to serial library
const int ledPin = 2;
const int trigPin = 5;
const int echoPin = 18;

bool isConnected = false;
bool isSetupDone = false;
bool isLedOn = false;
//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;  // Create a BT class instance

long duration;
float distanceCm;
float distanceInch;


void setup() {
  Serial.begin(115200);      // Starts the serial communication
  pinMode(trigPin, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);   // Sets the echoPin as an Input
  pinMode(ledPin, OUTPUT);   // Sets built-in led to output
  Serial.println("setup....");
  SerialBT.begin("ESP32BT BY JACOB");  // Bluetooth device name
}

void loop() {
  // Clears the trigPin

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Flag for setup is done
  if (!isSetupDone) {
    Serial.println("setup complete, running");
    isSetupDone = true;
  }
  // Check if a Bluetooth device is connected
  if (SerialBT.hasClient()) {
    if (!isConnected) {
      // A device just connected
      Serial.println("Bluetooth device is connected");
      SerialBT.println("The device started, now you can pair it with bluetooth!");
      for (int i = 0; i < 10; i++) {
        digitalWrite(ledPin, HIGH);
        delay(100);
        digitalWrite(ledPin, LOW);
        delay(200);
      }

      isConnected = true;
    }
  } else {
    isConnected = false;
  }

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distanceCm = duration * SOUND_SPEED / 2;
  if (distanceCm < 10) {
    digitalWrite(ledPin, HIGH);
    if (!isLedOn) {
      Serial.println("Distance is less than 10CM!");
      Serial.println("LED Turned ON Blyat!");
      SerialBT.println("Distance is less than 10CM!");
      SerialBT.println("LED Turned ON Blyat!");
      isLedOn = true;
    }
  } else {
    digitalWrite(ledPin, LOW);
    if (isLedOn) {
      Serial.println("LED OFF");
      SerialBT.println("LED OFF");
    }
    isLedOn = false;
  }
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);


  delay(1000);
}
