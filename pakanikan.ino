#include <Servo.h>
#include <virtuabotixRTC.h>
Servo myservo;
virtuabotixRTC myRTC(14, 16, 4);
#define echoPin 12
#define trigPin 5
long duration, distance;
void setup() {
  Serial.begin(57600);
  //    myRTC.setDS1302Time(00,57,8,6,06,11,2022);
  myservo.attach(0);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  rtc();
  myservo.write(0);
  if (myRTC.hours == 10 && myRTC.minutes == 40) {
    myservo.write(90);
    delay(1000);
    myservo.write(45);
    delay(1000);
  }
//hcsr();
}
