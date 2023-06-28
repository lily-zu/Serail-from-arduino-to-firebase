#include <FirebaseESP32.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <EEPROM.h>
FirebaseData firebaseData;
#define LED 7
int S;
unsigned long count = 0;
unsigned long tiempo_actual = 0;
unsigned long timeoff = 0;
const int idadd = 0;
char strget[30];
String id = "";
int Vresistor = A0;
int Vrdata = 0;
unsigned long timer;
String url = "";
const char* ssid = "AIFARM ROBOTICS FACTORY";
const char* password = "@AIFARM2022";
void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  Serial.setTimeout(200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  // ArduinoOTA.setHostname("myesp32");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
  .onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else  // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  })
  .onEnd([]() {
    Serial.println("\nEnd");
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  })
  .onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Firebase.begin("https://cam001-8dbdf-default-rtdb.firebaseio.com/", "gAPAtRAPSk3nJjRx7eVdg3Zb3QeLwAiNeFUbcfV8");
 // Firebase.begin("https://cam001-8dbdf-default-rtdb.firebaseio.com/", "oXMCZzYBLC4bzrQqPCKulp3i9tymz0SH9vF9n0fH");
  timer = millis();
}
void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readString ();
    S = data.toInt();
  }
  delay(50); digitalWrite(LED, 1);
  delay(50); digitalWrite(LED, 0);
  delay(100); Serial.println(S);
  Firebase.setString(firebaseData, "/L", S);
  //Firebase.setString(firebaseData, "/L/A", data);
}