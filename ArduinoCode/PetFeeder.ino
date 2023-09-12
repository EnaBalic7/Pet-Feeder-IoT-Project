#include <ESP8266WiFi.h>
#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>
#include <Servo.h>

#define FIREBASE_HOST "example.firebaseio.com"
#define FIREBASE_AUTH "abcdefghijklmnopqrstuvwxyz"
#define WIFI_SSID "WiFiSSID"                 // wifi name
#define WIFI_PASSWORD "WiFiPASSWORD"  //wifi password

const int TRIG_PIN  = 4; // D4 pin connected to Ultrasonic Sensor's TRIG pin
const int ECHO_PIN  = 0; // D3 pin connected to Ultrasonic Sensor's ECHO pin
const int SERVO_PIN = 2; // D4 connected to Servo Motor's pin
const int foodDispenseInterval = 10000; // 10 seconds
// 2 hours = 7200000 milliseconds
// 5 seconds = 5000 milliseconds
// 10 seconds = 10000 milliseconds

Servo myServo;

long microsecondsToCentimeters(long microseconds) {
    // Convert time duration to distance in centimeters
    // Speed of sound = 343 m/s = 34300 cm/s
    // Distance = (time / 2) * speed_of_sound
    return microseconds / 29 / 2;
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WIFI_SSID);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  if (Firebase.failed()) {
    Serial.print(Firebase.error());
  } else {
    Serial.println("Firebase Connected");
  }

 // Attach servo to pin
    myServo.attach(SERVO_PIN);

}

void dispenseFood() {
    myServo.write(30); 
    delay(500); 
    myServo.write(90);
}


void sendFoodLowNotification() {
    Firebase.setInt("foodLow", 1);
}

void sendFoodNotLowNotification() {
    Firebase.setInt("foodLow", 0);
}

void loop() {
  // put your main code here, to run repeatedly:

  
long duration, cm;

    int dispenseValue = Firebase.getInt("dispense");
    if (dispenseValue == 1) {
       dispenseFood();
       Firebase.setInt("dispense", 0);
       Firebase.setInt("foodDispensed", 1);
    }

    pinMode(TRIG_PIN, OUTPUT);
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    pinMode(ECHO_PIN, INPUT);
    duration = pulseIn(ECHO_PIN, HIGH);
    cm = microsecondsToCentimeters(duration);

    Firebase.setInt("distance", cm);
    Serial.print("Food level: ");
    Serial.print(cm);
    Serial.println(" cm");

    // Check if food level is low and send notification if needed
    // If distance from ultrasonic sensor to pet food is bigger than 15 cm
    if (cm > 15) {
        sendFoodLowNotification();
    }
    else {
        sendFoodNotLowNotification();
    }

    // Ensures that food is dispensed periodically at intervals defined by foodDispenseInterval
    unsigned long currentMillis = millis();
    static unsigned long previousDispenseMillis = 0;
    if (currentMillis - previousDispenseMillis >= foodDispenseInterval) {
        dispenseFood();
        previousDispenseMillis = currentMillis;
    }

    delay(5000);
}
