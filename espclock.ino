#include <SPI.h>
#include <ESP8266WiFi.h>

#define MAX7219_NUM_CHIPS   1  // 3 chips = 24 digits
#define MAX7219_CS_PIN     15  // pin the MAX7219's CS pin is connected to
#define MAX7219_BRIGHTNESS  3  // brightness level 0-15

#include <MAX7219_8_Digit_Driver.h>

MAX7219_8_Digit_Driver my_display(MAX7219_CS_PIN, MAX7219_NUM_CHIPS);

// Wifi
#include "mySettings.h"

// For NTP time
#include <NTPClient.h>
#include <WiFiUdp.h>

const long utcOffset = 3600;

WiFiUDP ntpUdp;
NTPClient timeClient(ntpUdp, "pool.ntp.org", utcOffset);

void setup() {
  Serial.begin(115200); delay(10);
  Serial.println();

  my_display.String_To_Buffer("   H1   ", MAX7219_BRIGHTNESS);

  // ssid & pass in settings.ino - DO NOT CHECK IN!
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to "); Serial.print(ssid); Serial.println(" ...");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("Connected, IP: "); Serial.println(WiFi.localIP());

  my_display.String_To_Buffer(WiFi.localIP().toString(), MAX7219_BRIGHTNESS);

  timeClient.begin();

  delay(3000);
}

void loop() {
  timeClient.update();

  int hour = timeClient.getHours();
  int min  = timeClient.getMinutes();
  int sec  = timeClient.getSeconds();

  String pad_hour = "E";
  String pad_min  = "E";
  String pad_sec  = "E";

  if (hour < 10) { pad_hour = String("0") + String(hour); }
  else           { pad_hour = String(hour); }

  if (min < 10)  { pad_min = String("0") + String(min); }
  else           { pad_min = String(min); }

  if (sec < 10)  { pad_sec = String("0") + String(sec); }
  else           { pad_sec = String(sec); }

  String dispTime = pad_hour + "." + pad_min + "." + pad_sec;

  // The decimal point does not count against the length of the character string
  my_display.String_To_Buffer(dispTime, MAX7219_BRIGHTNESS);

  delay(1000);
}
