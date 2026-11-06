#include "AdafruitIO_WiFi.h"

#define IO_USERNAME  "USERNAME_ADAFRUIT"
#define IO_KEY       "AIO_KEY_ANDA"
#define WIFI_SSID    "NAMA_WIFI_ANDA"
#define WIFI_PASS    "PASSWORD_WIFI_ANDA"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

AdafruitIO_Feed *feedNilai = io.feed("nilai");
AdafruitIO_Feed *feedLed   = io.feed("led");

#define LED_PIN 2
int nilai = 0;
unsigned long terakhir = 0;

// Jalan otomatis saat toggle di dashboard ditekan
void handleLed(AdafruitIO_Data *data) {
  Serial.print("Perintah LED: ");
  Serial.println(data->value());
  digitalWrite(LED_PIN, data->toPinLevel());
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  Serial.print("Menghubungkan ke Adafruit IO");
  io.connect();
  feedLed->onMessage(handleLed);

  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(io.statusText());
  feedLed->get();
}

void loop() {
  io.run();  

  if (millis() - terakhir > 5000) {  // kirim tiap 5 detik
    terakhir = millis();
    feedNilai->save(nilai);
    Serial.print("Kirim nilai = "); Serial.println(nilai);
    nilai++;
    if (nilai > 255) nilai = 0;
  }
}
