#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> //WIFI HARUS ON
#include <BlynkSimpleEsp8266.h>

#define BLYNK_TEMPLATE_ID "TMPL6LDzOd0GO"
#define BLYNK_TEMPLATE_NAME "CONTROLLER"
#define BLYNK_AUTH_TOKEN "dNiuTk5rYhJ_MZfBepVypLb-XgSIO6x-"

char auth[] = "dNiuTk5rYhJ_MZfBepVypLb-XgSIO6x-";
char ssid[] = "BOGOR PRIDE";
char pass[] = "mylord1010";

BlynkTimer timer;

int thermoDO = 4;
int thermoCS = 5;
int thermoCLK = 6;

#define thermoDO D4
#define thermoCS D5
#define thermoCLK D6

#include "max6675.h"
MAX6675 thermocouple (thermoCLK, thermoCS, thermoDO);

#define relay1 D0
#define relay2 D1
#define relay3 D2
#define relay4 D3

// Ini syntax untuk otomatisasi relay valve dan relay water heater
// BLYNK_WRITE(V0) { // Relay menyalakan air dan water heater secara otomatis
//   bool value1 = param.asInt();
//   if (value1 == 1) {
//     digitalWrite(relay1, HIGH);
//     Blynk.virtualWrite(V5, HIGH);
//     delay(10000);
//     digitalWrite(relay1, LOW);
//     Blynk.virtualWrite(V5, LOW);
//     // Blynk.virtualWrite(V0, LOW);
//   }
//   if(value1 == 1){
//     digitalWrite(relay2, HIGH);
//     Blynk.virtualWrite(V2, HIGH);
//     Blynk.virtualWrite(V6, HIGH);
//     delay(10000);
//     digitalWrite(relay2, LOW);
//     Blynk.virtualWrite(V0, LOW);
//     Blynk.virtualWrite(V6, LOW);
//   }
// }

BLYNK_WRITE(V0) {
  bool value1 = param.asInt();
  if (value1 == 1) {
    digitalWrite(relay1, HIGH);
    // Blynk.virtualWrite(V5, LOW);
    // AKTIFKAN CODE DIBAWAH JIKA INGIN RELAY 1 MENGGUNAKAN DELAY
    // delay(10000);
    // digitalWrite(relay1, HIGH);
    // Blynk.virtualWrite(V5, LOW);
    // Blynk.virtualWrite(V0, LOW);
  }else{
    digitalWrite(relay1, LOW);
    Blynk.virtualWrite(V5, LOW);
  }
}
BLYNK_WRITE(V2) {
  bool value2 = param.asInt();
  if (value2 == 1) {
    digitalWrite(relay2, HIGH);
    delay(10000); // Butuh riset, berapa ML air bisa didapatkan jika dalam waktu per 1 sec. Takutnya airnya kebanyakan saat diolah krn dpt merusak adonan briket.
    digitalWrite(relay2, LOW);
    Blynk.virtualWrite(V2, LOW);
  }
}
BLYNK_WRITE(V4) {
  bool value4 = param.asInt();
  if (value4 == 1) {
    digitalWrite(relay4, LOW);
  } else {
    digitalWrite(relay4, HIGH);
  }
}

void setup()
{
  // thermocouple.begin();
  timer.setInterval (2500L, sendSensor);

  pinMode (relay1, OUTPUT);
  pinMode (relay2, OUTPUT);
  pinMode (relay3, OUTPUT);
  pinMode (relay4, OUTPUT);

  // digitalWrite(relay1, HIGH);
  // digitalWrite(relay2, HIGH);
  // digitalWrite(relay3, HIGH);
  // digitalWrite(relay4, HIGH);

  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
}

void loop()
{
  BLYNK_WRITE(V0);
  float s = thermocouple.readCelsius();
  Serial.print("C = ");
  Serial.println(thermocouple.readCelsius());
  if(thermocouple.readCelsius() > 35.00){
    digitalWrite(relay1, LOW);
    Blynk.virtualWrite(V0, LOW);
    Blynk.virtualWrite(V5, LOW);
    // Blynk.logEvent("suhu", "Air Panas Anda Sudah Siap");
  }
    // digitalWrite(relay1, HIGH);
    // Blynk.logEvent("SUHU", "AIR Panas Anda Akan Siap Dalam Beberapa Menit Lagi. Mesin Akan Melanjutkan Proses Secara OTOMATIS");
  // else{
  //   digitalWrite(relay1, HIGH);
  // }
  // if(thermocouple.readCelsius() > 32.00){
    // digitalWrite(relay2, LOW);
  //   Blynk.logEvent("SUHU", "AIR Panas Anda Akan Siap Dalam Beberapa Menit Lagi. Mesin Akan Melanjutkan Proses Secara OTOMATIS");
  // }
  // else{
  //   digitalWrite(relay2, HIGH);
  // }
  // if(thermocouple.readCelsius() > 38.00){
  //   digitalWrite(relay3, LOW);
  //   Blynk.logEvent("NOTIFIKASI...", "AIR Panas Anda Akan Siap Dalam Beberapa Menit Lagi. Mesin Akan Melanjutkan Proses Secara OTOMATIS");
  // }else{
  //   digitalWrite(relay3, HIGH);
  // }
  // if(thermocouple.readCelsius() > 42.00){
  //   digitalWrite(relay4, LOW);
  //   Blynk.logEvent("NOTIFIKASI...", "AIR Panas Anda Akan Siap Dalam Beberapa Menit Lagi. Mesin Akan Melanjutkan Proses Secara OTOMATIS");
  // }else{
  //   digitalWrite(relay4, HIGH);
  // }

  Serial.print("F = ");
  Serial.println(thermocouple.readFahrenheit());
  delay(1000);

  Blynk.run();
  timer.run();
}
void sendSensor(){
  float s = thermocouple.readCelsius();
  if(isnan(s)){
    Serial.println("FAILED");
    return;
  }
  Blynk.virtualWrite(V1, s);
}
