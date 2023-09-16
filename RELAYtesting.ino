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
#define relay5 D7

BLYNK_WRITE(V2){
  bool value2 = param.asInt();
  if(value2 == 1){
    digitalWrite(relay2, HIGH);
    Blynk.virtualWrite(V6, HIGH);
  }else{
    digitalWrite(relay2, LOW);
    Blynk.virtualWrite(V6, LOW);
  }
}

BLYNK_WRITE(V3){
  bool value3 = param.asInt();
  if(value3 == 1){
    digitalWrite(relay3, HIGH);
    Blynk.virtualWrite(V7, HIGH);
  }else{
    digitalWrite(relay3, LOW);
    Blynk.virtualWrite(V7, LOW);
  }
}

BLYNK_WRITE(V0) {
  bool value1 = param.asInt();
  bool value2 = param.asInt();
  bool value3 = param.asInt();
  if (value1 == 1) {
    digitalWrite(relay1, HIGH); // relay air kran ke wadah air
    Blynk.virtualWrite(V5, HIGH);
    delay(5000); // waktunya hanya contoh dan bisa di ubah
    digitalWrite(relay1, LOW);
    Blynk.virtualWrite(V0, LOW); // mengirim sinyal ke blynk agar sinkron
    Blynk.virtualWrite(V5, LOW);
  }
  if(value1 == 1){
    digitalWrite(relay2, HIGH); // relay water heater, relay akan OFF berdasarkan sensor suhu jika >= 80c
    Blynk.virtualWrite(V6, HIGH);
    Blynk.virtualWrite(V0, HIGH);
    Blynk.logEvent("suhu", "Air sedang dipanaskan. Hindari Kontak Langsung, Benda PANAS");
  }
  if(value1 == 1){
    digitalWrite(relay3, HIGH); // relay valve air panas ke mixer
    Blynk.virtualWrite(V7, HIGH);
    Blynk.virtualWrite(V0, HIGH);
    delay(3000); // waktu ini butuh observasi lanjutan. Butuh berapa detik untuk setiap berapa ML. Soalnya air tidak boleh terlalu banyak karena bisa merusak adonan bahan briket
    digitalWrite(relay3, LOW);
    Blynk.virtualWrite(V7, LOW);
    Blynk.virtualWrite(V0, LOW);
  }
}

BLYNK_WRITE(V8){
  bool value5 = param.asInt();
  if(value5 == 1){
    digitalWrite(relay5, HIGH);
  }else{
    digitalWrite(relay5, LOW);
  }
}

// BLYNK_WRITE(V2) {
//   bool value2 = param.asInt();
//   if (value2 == 1) {
//     digitalWrite(relay2, HIGH);
//     // delay(10000); // Butuh riset, berapa ML air bisa didapatkan jika dalam waktu per 1 sec. Takutnya airnya kebanyakan saat diolah krn dpt merusak adonan briket.
//     // digitalWrite(relay2, LOW);
//     // Blynk.virtualWrite(V2, LOW);
//   }
//   else{
//     digitalWrite(relay2, LOW);
//     Blynk.virtualWrite(V2, LOW);
//   }
// }

void setup()
{
  // thermocouple.begin();
  timer.setInterval (2500L, sendSensor);

  pinMode (relay1, OUTPUT);
  pinMode (relay2, OUTPUT);
  pinMode (relay3, OUTPUT);
  pinMode (relay4, OUTPUT);
  pinMode (relay5, OUTPUT);

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
  if(thermocouple.readCelsius() > 65.00){ // Ini contoh suhu sementara, nanti bisa diubah menjadi 80c
    digitalWrite(relay2, LOW);
    Blynk.virtualWrite(V2, LOW);
    Blynk.virtualWrite(V6, LOW);
    Blynk.virtualWrite(V0, LOW);
    Blynk.logEvent("suhu", "Air Panas Anda Sudah Siap"); // AKAN MENGIRIM NOTIFIKASI
    // digitalWrite(relay3, HIGH);
    // Blynk.virtualWrite(V7, HIGH);
    // Blynk.virtualWrite(V0, HIGH);
    // delay(3000);
    // digitalWrite(relay3, LOW);
    // Blynk.virtualWrite(V7, LOW);
    // Blynk.virtualWrite(V0, LOW);
  }

  Serial.print("F = ");// sebenarnya F tidak terlalu dibutuhkan karena sudah ada C
  Serial.println(thermocouple.readFahrenheit());
  delay(1000);

  Blynk.run();
  timer.run();
}
void sendSensor(){//syntax pengiriman value sensor
  float s = thermocouple.readCelsius();
  if(isnan(s)){
    Serial.println("FAILED");
    return;
  }
  Blynk.virtualWrite(V1, s);
}
