#include "CTBot.h";
CTBot myBot;
#include <EEPROM.h>
#include <virtuabotixRTC.h>
virtuabotixRTC myRTC(16, 5, 4); // clk,dat,rst
#include <Servo.h>
Servo myservo;
//String ssid = "AndroidAP2E90";
//String pass = "mltr8744";
String ssid = "IOT";
String pass = "12345678";
String token = "5711953223:AAH9wd-Ip9YIB22nSHojh0BqcWkmB5PyReY";
const int id = 1458158972;
#define echoPin 0
#define trigPin 2
int tanggal, bulan, tahun, jam, menit, detik;
int jam1, menit1, jam2, menit2;
int makan, b, c;
int jamMakan, menitMakan;
int duration;
int distance;
char buf[10];
int logic = 0;
//int makan;
int x = 0;
String arrData[4];
//int berhasil = 0;
void setup() {

  Serial.begin(9600);
  //  Serial.println("Memulai bot telegram");
  myservo.attach(14);
  myservo.write(60);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);

  if (myBot.testConnection()) {
    Serial.println("koneksi terhubung");
    myBot.sendMessage(id, "koneksi terhubung");
  }
  else {
    Serial.println("koneksi gagal");
    myBot.sendMessage(id, "koneksi gagal");
  }
  EEPROM.begin(6);
  jam1       = EEPROM.read(0);                  //Baca EEPROM
  menit1     = EEPROM.read(1);
  jam2       = EEPROM.read(2);
  menit2     = EEPROM.read(3);
  jamMakan   = EEPROM.read(4);
  menitMakan = EEPROM.read(5);
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  //rtc
  myRTC.updateTime();
  tanggal = myRTC.dayofmonth;
  bulan = myRTC.month;
  tahun = myRTC.year;
  jam = myRTC.hours;
  menit = myRTC.minutes;
  detik = myRTC.seconds;

  Serial.println(String() + tanggal + "-" + bulan + "-" + tahun);
  Serial.println(String() + jam + ":" + menit + ":" + detik);
  Serial.println();
  delay(1000);
  //hcsr
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58.2;
  Serial.println(distance);
  //  sprintf(buf, "%d", distance);
  //bot
  TBMessage msg;
  if (myBot.getNewMessage(msg)) {
    Serial.println("pesan masuk: " + msg.text);
    String pesan = msg.text;

    int index = 0;
    arrData[0] = "";
    arrData[1] = "";
    arrData[2] = "";
    arrData[3] = "";
    for (int i = 0; i < pesan.length(); i++) {
      char delimeter = '#';
      if (pesan[i] != delimeter)
        arrData[index] += pesan[i];                        //Gabungkan isi data array dengan karakter yg sedang dibaca
      else
        index = index + 1;
      delay(10);
    }
    if (index== 3) {
      jam1      = arrData[0].toInt();
      menit1    = arrData[1].toInt();
      jam2      = arrData[2].toInt();
      menit2    = arrData[3].toInt();
      EEPROM.write(0, jam1);                                 //Simpan data ke EEPROM
      EEPROM.write(1, menit1);
      EEPROM.write(2, jam2);
      EEPROM.write(3, menit2);
      EEPROM.commit();
      myBot.sendMessage(id, "Pembaharuan Berhasil");
    }
    if (pesan == "Cek pakan") {
      myBot.sendMessage(id, "Sedang mengecek pakan");
    }
    if (distance <= 6) {
      myBot.sendMessage(id, "pakan belum penuh");
      logic = 0;
    } else if (distance >= 7) {
      logic = 1;
    }
    if (logic == 1) {
      //if(pesan =="Menu"){
      //      atas:
      myBot.sendMessage(id, "Pakan masih tersedia \nsilahkan kirim: \n1 = beri pakan ikan sekarang \n2 = pengaturan jam pakan ikan \n3 = Pengaturang jam pemberian pakan");
      if (pesan == "1") {
        while (1) {
          myservo.write(0);                                    //Bergerak ke posisi 180 derajat
          delay(1000);
          while (1) {
            myBot.sendMessage(id, "Pemberian Makan Berhasil");
            myservo.write(60);
            delay(1000);
            break;
          }
          break;
        }

      }
      else if (pesan == "2") {
        myBot.sendMessage(id, "Silakan masukkan jam & menit \nFormat : Jam1#Menit1#Jam2#Menit2");

      }
      else if (pesan == "3") {
        myBot.sendMessage(id, String() + "Jam Makan1 = " + jam1 + ":" + menit1 + "\nJam Makan2 = " + jam2 + ":" + menit2);
      }
    }
  }
  delay(1000);
  if (jam == jam1 & menit == menit1 & b != 1) {
    kasih_pakan(2);
    myBot.sendMessage(id, "Pemberian Makan Berhasil (Jam Makan 1)");
    jamMakan = jam1; menitMakan = menit1; b = 1; c = 0;
    EEPROM.write(4, jamMakan);
    EEPROM.write(5, menitMakan);
    EEPROM.commit();
    delay(100);
  }
  if (jam == jam2 & menit == menit2 & c != 1) {
    kasih_pakan(2);                                  //panggil void kasih pakan
    Serial.println("Pemberian Makan Berhasil (Jam Makan 2)");
    myBot.sendMessage(id, "Pemberian Makan Berhasil (Jam Makan 2)");
    jamMakan = jam2; menitMakan = menit2; c = 1 ; b = 0;
    EEPROM.write(4, jamMakan);
    EEPROM.write(5, menitMakan);
    EEPROM.commit();
    delay(100);
  }
}
void kasih_pakan(int hitung) {
  for (int i = 0; i < hitung; i++) {
    myservo.write(0);
    delay(1000);
    myservo.write(60);
    delay(1000);
  }
}

//  if (makan == 1) {
//    //bawah:
//    kasih_pakan(1);                                        //panggil void kasih pakan
//    myBot.sendMessage(id, "Pemberian Makan Berhasil");
//    makan = 0;
//    delay(100);
//    //      goto atas;
//  }//if
//}
//void kasih_pakan(int jumlah) {
// //for
