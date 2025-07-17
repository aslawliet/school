#include <WiFi.h>
#include <HTTPClient.h>
#include <SPIFFS.h>
#include "BluetoothA2DPSink.h"
#include "AudioFileSourceSPIFFS.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

// WiFi credentials
const char* ssid = "ABHI WIFI 2.4GHZ";
const char* password = "Indra@68";

// Flask server
const char* mp3_url = "http://192.168.0.104:5000/get_mp3";

AudioGeneratorMP3 *mp3;
AudioFileSourceSPIFFS *file;
AudioOutputI2S *out;
BluetoothA2DPSink a2dp_sink;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Mount SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  // WiFi connect
  Serial.printf("Connecting to %s...\n", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.println(WiFi.localIP());

  // Download MP3
  Serial.println("Downloading MP3 from server...");
  HTTPClient http;
  http.begin(mp3_url);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    File f = SPIFFS.open("/speech.mp3", FILE_WRITE);
    if (!f) {
      Serial.println("Failed to open file for writing");
      return;
    }
    http.writeToStream(&f);
    f.close();
    Serial.println("MP3 saved to SPIFFS.");
  } else {
    Serial.printf("HTTP GET failed. Code: %d\n", httpCode);
    return;
  }
  http.end();

  // Start A2DP BT audio
  a2dp_sink.start("ESP32-Audio");

  delay(2000);  // Time to pair BT speaker

  // Begin MP3 playback
  file = new AudioFileSourceSPIFFS("/speech.mp3");
  out = new AudioOutputI2S(0, 1);  // Output to BT sink
  out->SetOutputModeMono(true);   // Mono = safer for small speakers
  out->SetGain(1.0);              // Volume boost
  mp3 = new AudioGeneratorMP3();

  if (!mp3->begin(file, out)) {
    Serial.println("MP3 playback failed!");
    return;
  }

  Serial.println("Playing MP3...");
}

void loop() {
  if (mp3 && mp3->isRunning()) {
    mp3->loop();
  } else {
    delay(500);
  }
}
