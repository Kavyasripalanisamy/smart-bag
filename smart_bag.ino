#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
int ledPin = 13; //definition digital 8 pins as pin to control the LED
volatile int finger_status = -1;
int state = 0;
const int pin = 9;
int sensorPin = A0;
int sensorValue = 0;
int led = 12;
float gpslat, gpslon;
TinyGPS gps;
SoftwareSerial sgps(4, 5);
SoftwareSerial sgsm(2, 3);
SoftwareSerial mySerial(11, 12); // TX/RX on fingerprint sensor
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
void setup()
{
  sgsm.begin(9600);
  sgps.begin(9600);
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(ledPin, OUTPUT);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");

  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
}
void finger_data()
{
    finger_status = getFingerprintIDez();
    if (finger_status != -1 and finger_status != -2) {
      Serial.print("Match");
      digitalWrite(ledPin, HIGH);
      delay(1000);
      digitalWrite(ledPin, LOW);
    } else {
      if (finger_status == -2) {
        for (int ii = 0; ii < 5; ii++) {
          Serial.print("Not Match");
          digitalWrite(ledPin, LOW);
        }
      }
    }
    delay(50);            //don't ned to run this at full speed.
  }

  // returns -1 if failed, otherwise returns ID #
  int getFingerprintIDez() {
    uint8_t p = finger.getImage();
    if (p != 2) {
      Serial.println(p);
    }
    if (p != FINGERPRINT_OK)  return -1;

    p = finger.image2Tz();
    if (p != 2) {
      Serial.println(p);
    }
    if (p != FINGERPRINT_OK)  return -1;

    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK)  return -2;

    // found a match!
    Serial.print("Found ID #"); Serial.print(finger.fingerID);
    Serial.print(" with confidence of "); Serial.println(finger.confidence);
    return finger.fingerID;
  }
void loop()
{
  sgps.listen();
  while (sgps.available())
  {
    int c = sgps.read();
    if (gps.encode(c))
    {
      gps.f_get_position(&gpslat, &gpslon);
    }
  }
  if (digitalRead(pin) == HIGH && state == 0) {
    sgsm.listen();
    sgsm.print("\r");
    delay(1000);
    sgsm.print("AT+CMGF=1\r");
    delay(1000);
    /*Replace XXXXXXXXXX to 10 digit mobile number &
      ZZ to 2 digit country code*/
    sgsm.print("AT+CMGS=\"+919787878161\"\r");
    delay(1000);
    //The text of the message to be sent.
    sgsm.print("https://www.google.com/maps/?q=");
    sgsm.print(gpslat, 6);
    sgsm.print(",");
    sgsm.print(gpslon, 6);
    delay(1000);
    sgsm.write(0x1A);
    delay(1000);
    state = 1;
  }
  if (digitalRead(pin) == LOW) {
    state = 0;
  }
  delay(100);
  void bright_ness();
  void finger_data();
}
void bright_ness()
{
  Serial.println("Welcome to LDR tutorial");
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  if (sensorValue > 100)
  {
    Serial.println("LED light on");
    digitalWrite(led, HIGH);
    delay(1000);
  }
  digitalWrite(led, LOW);
  delay(sensorValue);
  // put your main code here, to run repeatedly:
}
