// GxEPD_HD_SerialFlash_Loader : Display Library example for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
//
// Display Library based on Demo Example available from Good Display
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD_HD

// Supporting Arduino Forum Topics:
// Waveshare e-paper displays with SPI: http://forum.arduino.cc/index.php?topic=487007.0
// Good Display ePaper for Arduino : https://forum.arduino.cc/index.php?topic=436411.0
//
// this example uses the SerialFlash library from: https://github.com/PaulStoffregen/SerialFlash
// with a modification for use with ESP32 or the STM32 package available here: https://github.com/ZinggJM/SerialFlash
// download it as .zip file and install with Library Mananger method "Add .ZIP Library..."
//
// this example can run on ESP6266 or ESP32 using WiFi connection

#include <SerialFlash.h>
#include <SPI.h>

// digital pin for flash chip CS pin:
const int FlashChipSelect = SS; // for standard slave select pin
//const int FlashChipSelect = 5; // use D1 on my Wemos D1 mini wired for e-papers
//const int FlashChipSelect = 17; // as used with my ESP32 breadboard

#if defined (ESP8266)
#include <ESP8266WiFi.h>
#define USE_BearSSL true
#endif

#include <WiFiClient.h>
#include <WiFiClientSecure.h>

const char* ssid     = "........";
const char* password = "........";
const int httpPort  = 80;
const int httpsPort = 443;
const char* fp_api_github_com = "35 85 74 EF 67 35 A7 CE 40 69 50 F3 C0 F6 80 CF 80 3B 2E 19";
const char* fp_github_com     = "ca 06 f5 6b 25 8b 7a 0d 4f 2b 05 47 09 39 47 86 51 15 19 84";
#if USE_BearSSL
const char fp_rawcontent[20]  = {0xcc, 0xaa, 0x48, 0x48, 0x66, 0x46, 0x0e, 0x91, 0x53, 0x2c, 0x9c, 0x7c, 0x23, 0x2a, 0xb1, 0x74, 0x4d, 0x29, 0x9d, 0x33};
#else
const char* fp_rawcontent     = "cc aa 48 48 66 46 0e 91 53 2c 9c 7c 23 2a b1 74 4d 29 9d 33";
#endif
const char* host_rawcontent   = "raw.githubusercontent.com";
const char* path_rawcontent   = "/ZinggJM/GxEPD2/master/extras/bitmaps/";
const char* path_prenticedavid   = "/prenticedavid/MCUFRIEND_kbv/master/extras/bitmaps/";

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("GxEPD2_SerialFlash_Loader");

#ifdef RE_INIT_NEEDED
  WiFi.persistent(true);
  WiFi.mode(WIFI_STA); // switch off AP
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.disconnect();
#endif

  if (!WiFi.getAutoConnect() || ( WiFi.getMode() != WIFI_STA) || ((WiFi.SSID() != ssid) && String(ssid) != "........"))
  {
    Serial.println();
    Serial.print("WiFi.getAutoConnect()=");
    Serial.println(WiFi.getAutoConnect());
    Serial.print("WiFi.SSID()=");
    Serial.println(WiFi.SSID());
    WiFi.mode(WIFI_STA); // switch off AP
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
  }
  int ConnectTimeout = 30; // 15 seconds
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    Serial.print(WiFi.status());
    if (--ConnectTimeout <= 0)
    {
      Serial.println();
      Serial.println("WiFi connect timeout");
      return;
    }
  }
  Serial.println();
  Serial.println("WiFi connected");

  // Print the IP address
  Serial.println(WiFi.localIP());
  Serial.println();

  // comment out for standalone use
  waitForStartConfirmation();

  if (!SerialFlash.begin(FlashChipSelect))
  {
    Serial.println("Unable to access SPI Flash chip");
    SPI.end(); // release SPI pins
    pinMode(FlashChipSelect, INPUT); // CS also!
    return;
  }
  Serial.println("SerialFlash started");
  eraseSerialFlash();
  listFiles();
  downloadBitmaps_200x200();
  downloadBitmaps_other();
  downloadBitmaps_test();
  listFiles();
  SPI.end(); // release SPI pins
  pinMode(FlashChipSelect, INPUT); // CS also!
  Serial.println("GxEPD2_SerialFlash_Loader done");
}

void loop()
{
}

void waitForStartConfirmation()
{
  bool ok = false;
  while (!ok)
  {
    Serial.print("type OK to allow start (SPI access) : ");
    while (Serial.available() < 2) yield();
    String response = Serial.readString();
    Serial.println(response);
    ok = response.indexOf("OK") >= 0;
  }
}

void listFiles()
{
  Serial.println("All Files on SPI Flash chip:");
  SerialFlash.opendir();
  while (1)
  {
    char filename[64];
    uint32_t filesize;

    if (SerialFlash.readdir(filename, sizeof(filename), filesize))
    {
      Serial.print("  ");
      Serial.print(filename);
      spaces(20 - strlen(filename));
      Serial.print("  ");
      Serial.print(filesize);
      Serial.print(" bytes");
      Serial.println();
    }
    else
    {
      Serial.println("no more files...");
      break; // no more files
    }
  }
}

void spaces(int num)
{
  for (int i = 0; i < num; i++)
  {
    Serial.print(" ");
  }
}

void downloadBitmaps_200x200()
{
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "logo200x200.bmp", fp_rawcontent, "logo200x200.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "first200x200.bmp", fp_rawcontent, "first200x200.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "second200x200.bmp", fp_rawcontent, "second200x200.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "third200x200.bmp", fp_rawcontent, "third200x200.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "fourth200x200.bmp", fp_rawcontent, "fourth200x200.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "fifth200x200.bmp", fp_rawcontent, "fifth200x200.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "sixth200x200.bmp", fp_rawcontent, "sixth200x200.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "seventh200x200.bmp", fp_rawcontent, "seventh200x200.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "eighth200x200.bmp", fp_rawcontent, "eighth200x200.bmp");
}

void downloadBitmaps_other()
{
  downloadFile_HTTP("www.squix.org", "/blog/wunderground/", "chanceflurries.bmp", "chanceflurries.bmp");
  downloadFile_HTTPS(host_rawcontent, path_prenticedavid, "betty_1.bmp", fp_rawcontent, "betty_1.bmp");
  downloadFile_HTTPS(host_rawcontent, path_prenticedavid, "betty_4.bmp", fp_rawcontent, "betty_4.bmp");
  downloadFile_HTTPS(host_rawcontent, path_prenticedavid, "marilyn_240x240x8.bmp", fp_rawcontent, "marilyn_240x240x8.bmp");
  downloadFile_HTTPS(host_rawcontent, path_prenticedavid, "miniwoof.bmp", fp_rawcontent, "miniwoof.bmp");
  //downloadFile_HTTPS(host_rawcontent, path_prenticedavid, "test.bmp", fp_rawcontent, "test.bmp");
  downloadFile_HTTPS(host_rawcontent, path_prenticedavid, "tiger.bmp", fp_rawcontent, "tiger.bmp");
  downloadFile_HTTPS(host_rawcontent, path_prenticedavid, "tiger_178x160x4.bmp", fp_rawcontent, "tiger_178x160x4.bmp");
  downloadFile_HTTPS(host_rawcontent, path_prenticedavid, "tiger_240x317x4.bmp", fp_rawcontent, "tiger_240x317x4.bmp");
  downloadFile_HTTPS(host_rawcontent, path_prenticedavid, "tiger_320x200x24.bmp", fp_rawcontent, "tiger_320x200x24.bmp");
  //downloadFile_HTTPS(host_rawcontent, path_prenticedavid, "tiger16T.bmp", fp_rawcontent, "tiger16T.bmp");
  downloadFile_HTTPS(host_rawcontent, path_prenticedavid, "woof.bmp", fp_rawcontent, "woof.bmp");
}

void downloadBitmaps_test()
{
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "output5.bmp", fp_rawcontent, "output5.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "output6.bmp", fp_rawcontent, "output6.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "tractor_1.bmp", fp_rawcontent, "tractor_1.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "tractor_4.bmp", fp_rawcontent, "tractor_4.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "tractor_8.bmp", fp_rawcontent, "tractor_8.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "tractor_11.bmp", fp_rawcontent, "tractor_11.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "tractor_44.bmp", fp_rawcontent, "tractor_44.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "tractor_88.bmp", fp_rawcontent, "tractor_88.bmp");
}

void downloadFile_HTTP(const char* host, const char* path, const char* filename, const char* target)
{
  WiFiClient client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpPort))
  {
    Serial.println("connection failed");
    return;
  }
  Serial.print("requesting URL: ");
  Serial.println(String("http://") + host + path + filename);
  client.print(String("GET ") + path + filename + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: GxEPD2_SerialFlash_Loader\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");
  bool ok = false;
  int32_t content_length = 0;
  while (client.connected() || client.available())
  {
    String line = client.readStringUntil('\n');
    if (!ok)
    {
      ok = line.startsWith("HTTP/1.1 200 OK");
      if (ok) Serial.println(line);
      //if (!ok) Serial.println(line);
    }
    if (!ok) Serial.println(line);
    //Serial.println(line);
    if (line.startsWith("Content-Length: "))
    {
      content_length = line.substring(16, line.length()).toInt();
      Serial.print("content_length is "); Serial.println(content_length);
    }
    if (line == "\r")
    {
      Serial.println("headers received");
      break;
    }
  }
  if (!ok) return;
  uint8_t buffer[512];
#if defined(ESP8266)
  client.peekBytes(buffer, 2);
  Serial.write(buffer[0]); Serial.write(buffer[1]); Serial.println();
#endif
  size_t total = 0;
  //if (!SerialFlash.createErasable(target, content_length)) // would use much more space
  if (!SerialFlash.create(target, content_length))
  {
    Serial.print(target); Serial.println(" create failed");
    return;
  }
  SerialFlashFile file = SerialFlash.open(target);
  if (!file)
  {
    Serial.print(target); Serial.println(" open failed");
    return;
  }
  while (client.connected() || client.available())
  {
    size_t available = client.available();
    size_t fetch = available <= sizeof(buffer) ? available : sizeof(buffer);
    if (fetch > 0)
    {
      size_t got = client.read(buffer, fetch);
      file.write(buffer, got);
      total += got;
    }
    delay(1); // yield();
  }
  file.close();
  Serial.print("done, "); Serial.print(total); Serial.println(" bytes transferred");
}

void downloadFile_HTTPS(const char* host, const char* path, const char* filename, const char* fingerprint, const char* target)
{
  // Use WiFiClientSecure class to create TLS connection
#if USE_BearSSL
  BearSSL::WiFiClientSecure client;
#else
  WiFiClientSecure client;
#endif
  Serial.println(); Serial.print("downloading file \""); Serial.print(filename);  Serial.println("\"");
  Serial.print("connecting to "); Serial.println(host);
#if USE_BearSSL
  if (fingerprint) client.setFingerprint((uint8_t*)fingerprint);
#endif
  if (!client.connect(host, httpsPort))
  {
    Serial.println("connection failed");
    return;
  }
#if defined (ESP8266) && !USE_BearSSL
  if (fingerprint)
  {
    if (client.verify(fingerprint, host))
    {
      Serial.println("certificate matches");
    }
    else
    {
      Serial.println("certificate doesn't match");
      return;
    }
  }
#endif
  Serial.print("requesting URL: ");
  Serial.println(String("https://") + host + path + filename);
  client.print(String("GET ") + path + filename + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: GxEPD2_SerialFlash_Loader\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");
  bool ok = false;
  int32_t content_length = 0;
  while (client.connected() || client.available())
  {
    String line = client.readStringUntil('\n');
    if (!ok)
    {
      ok = line.startsWith("HTTP/1.1 200 OK");
      if (ok) Serial.println(line);
      //if (!ok) Serial.println(line);
    }
    if (!ok) Serial.println(line);
    //Serial.print("line: "); Serial.println(line);
    if (line.startsWith("Content-Length: "))
    {
      content_length = line.substring(16, line.length()).toInt();
      Serial.print("content_length is "); Serial.println(content_length);
    }
    if (line == "\r")
    {
      Serial.println("headers received");
      break;
    }
  }
  if (!ok) return;
  //return;
  uint8_t buffer[512];
#if defined(ESP8266)
  client.peekBytes(buffer, 2);
  Serial.write(buffer[0]); Serial.write(buffer[1]); Serial.println();
#endif
  size_t total = 0;
  //if (!SerialFlash.createErasable(target, content_length)) // would use much more space
  if (!SerialFlash.create(target, content_length))
  {
    Serial.print(target); Serial.println(" create failed");
    return;
  }
  SerialFlashFile file = SerialFlash.open(target);
  if (!file)
  {
    Serial.print(target); Serial.println(" open failed");
    return;
  }
  while (client.connected() || client.available())
  {
    // this doesn't work as expected, but it helps for long downloads
    int32_t start = millis();
    for (int16_t t = 0, dly = 50; t < 20; t++, dly += 50)
    {
      if (!(client.connected() || client.available())) break;
      if (client.available()) break; // read would not recover after having returned 0
      delay(dly);
    }
    if (!(client.connected() || client.available())) break;
    int32_t elapsed = millis() - start;
    if (elapsed > 250)
    {
      Serial.print("waited for available "); Serial.print(millis() - start); Serial.print(" ms @ "); Serial.println(total);
    }
    size_t available = client.available();
    if (0 == available)
    {
      Serial.print("download error: timeout on available() after "); Serial.print(total); Serial.println(" bytes");
      break; // don't hang forever
    }
    size_t fetch = available <= sizeof(buffer) ? available : sizeof(buffer);
    if (fetch > 0)
    {
      size_t got = client.read(buffer, fetch);
      file.write(buffer, got);
      total += got;
    }
    delay(1); // yield();
    if (total > 30000) delay(250); // helps for long downloads
  }
  file.close();
  Serial.print("done, "); Serial.print(total); Serial.println(" bytes transferred");
}

void eraseSerialFlash()
{
  unsigned char id[5];
  SerialFlash.readID(id);
  unsigned long size = SerialFlash.capacity(id);
  unsigned long startMillis = millis();

  if (size > 0)
  {
    Serial.print("Flash Memory has ");
    Serial.print(size);
    Serial.println(" bytes.");
    Serial.println("Erasing ALL Flash Memory:");
    // Estimate the (lengthy) wait time.
    Serial.print("  estimated wait: ");
    int seconds = (float)size / eraseBytesPerSecond(id) + 0.5;
    Serial.print(seconds);
    Serial.println(" seconds.");
    Serial.println("  Yes, full chip erase is SLOW!");
    SerialFlash.eraseAll();
    unsigned long dotMillis = millis();
    unsigned char dotcount = 0;
    while (SerialFlash.ready() == false)
    {
      if (millis() - dotMillis > 1000)
      {
        dotMillis = dotMillis + 1000;
        Serial.print(".");
        dotcount = dotcount + 1;
        if (dotcount >= 60)
        {
          Serial.println();
          dotcount = 0;
        }
      }
      yield();
    }
    if (dotcount > 0) Serial.println();
    Serial.println("Erase completed");
    unsigned long elapsed = millis() - startMillis;
    Serial.print("  actual wait: ");
    Serial.print(elapsed / 1000ul);
    Serial.println(" seconds.");
  }
}

float eraseBytesPerSecond(const unsigned char *id)
{
  if (id[0] == 0x20) return 152000.0; // Micron
  if (id[0] == 0x01) return 500000.0; // Spansion
  if (id[0] == 0xEF) return 419430.0; // Winbond
  if (id[0] == 0xC2) return 279620.0; // Macronix
  return 320000.0; // guess?
}
