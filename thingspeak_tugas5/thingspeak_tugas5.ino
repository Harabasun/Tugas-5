#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <ThingSpeak.h>

const char* ssid     = "NAMA_WIFI_ANDA";
const char* password = "PASSWORD_WIFI_ANDA";

unsigned long channelID = ;       
const char* writeKey = "";  
const char* readKey  = "";  

#define LED_PIN 2
WiFiClient client;
int nilai = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Berhasil!");
  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client);
}

void loop() {
  // 1. Kirim nilai ke Field 1
  ThingSpeak.setField(1, nilai);
  int kode = ThingSpeak.writeFields(channelID, writeKey);
  Serial.print("Desimal: "); Serial.print(nilai);
  Serial.print(" | Biner: "); Serial.print(nilai, BIN);
  Serial.println(kode == 200 ? " -> terkirim" : " -> GAGAL");

  // 2. Baca Field 2 untuk kontrol LED
  long led = ThingSpeak.readLongField(channelID, 2, readKey);
  digitalWrite(LED_PIN, led == 1 ? HIGH : LOW);

  nilai++;
  if (nilai > 255) nilai = 0;
  delay(20000);  // ThingSpeak gratis: minimal 15 detik per kirim
}
