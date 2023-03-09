// GxEPD_HD_Spiffs_Loader : example for HD e-Paper displays from Dalian Good Display Inc. (parallel interface).
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
// Good Display ePaper for Arduino: https://forum.arduino.cc/index.php?topic=436411.0

#if defined(ESP32)
#include "SPIFFS.h"
// set formatOnFail = true for initial format of ESP32 SPIFFS (e.g. if error occured)
const bool formatOnFail = true;
#endif

#include <FS.h>

#if defined (ESP8266)
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#endif

#include <WiFiClient.h>
#include <WiFiClientSecure.h>

const char* ssid     = "........";
const char* password = "........";
const int httpPort  = 80;
const int httpsPort = 443;
// note: the certificates have been moved to a separate header file, as R"CERT( destroys IDE Auto Format capability

#include "GxEPD_HD_github_raw_certs.h"

const char* certificate_rawcontent = cert_DigiCert_TLS_RSA_SHA256_2020_CA1; // ok, should work until 2031-04-13 23:59:59
//const char* certificate_rawcontent = github_io_chain_pem_first;  // ok, should work until Tue, 21 Mar 2023 23:59:59 GMT
//const char* certificate_rawcontent = github_io_chain_pem_second;  // ok, should work until Tue, 21 Mar 2023 23:59:59 GMT
//const char* certificate_rawcontent = github_io_chain_pem_third;  // ok, should work until Tue, 21 Mar 2023 23:59:59 GMT

const char* host_rawcontent   = "raw.githubusercontent.com";
const char* path_rawcontent   = "/ZinggJM/GxEPD2/master/extras/bitmaps/";
const char* path_prenticedavid   = "/prenticedavid/MCUFRIEND_kbv/master/extras/bitmaps/";
const char* fp_rawcontent     = "8F 0E 79 24 71 C5 A7 D2 A7 46 76 30 C1 3C B7 2A 13 B0 01 B2"; // as of 29.7.2022

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("GxEPD_HD_Spiffs_Loader");

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

  setClock();

#if defined(ESP32)
  SPIFFS.begin(formatOnFail);
  Serial.println("SPIFFS started");
#else
  LittleFS.begin();
  Serial.println("LittleFS started");
#endif
  //listFiles();
  //deleteFiles();
  spiffs_sizes();
  //listFiles();
  downloadBitmaps_200x200();
  spiffs_sizes();
  downloadBitmaps_other();
  spiffs_sizes();
  downloadBitmaps_test();
  listFiles();
  spiffs_sizes();
  Serial.println("GxEPD_HD_Spiffs_Loader done");
}

void loop()
{
}

void downloadFile_HTTPS(const char* host, const char* path, const char* filename, const char* fingerprint, const char* target, const char* certificate = certificate_rawcontent);

void spiffs_sizes()
{
#if defined(ESP32)
  size_t total = SPIFFS.totalBytes();
  size_t used = SPIFFS.usedBytes();
  Serial.println();
  Serial.print("SPIFFS: used: "); Serial.print(used); Serial.print(" free: "); Serial.print(total - used);  Serial.print(" total: "); Serial.println(total);
#endif
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
  //downloadFile_HTTPS(host_rawcontent, path_rawcontent, "tractor_8.bmp", fp_rawcontent, "tractor_8.bmp"); // format 1: BI_RLE8 is not supported
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "tractor_11.bmp", fp_rawcontent, "tractor_11.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "tractor_44.bmp", fp_rawcontent, "tractor_44.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "tractor_88.bmp", fp_rawcontent, "tractor_88.bmp");
}

#if defined(ESP32)
void deleteFiles()
{
  SPIFFS.remove("/logo200x200.bmp");
  SPIFFS.remove("/first200x200.bmp");
  SPIFFS.remove("/second200x200.bmp");
  SPIFFS.remove("/third200x200.bmp");
  SPIFFS.remove("/fourth200x200.bmp");
  SPIFFS.remove("/fifth200x200.bmp");
  SPIFFS.remove("/sixth200x200.bmp");
  SPIFFS.remove("/seventh200x200.bmp");
  SPIFFS.remove("/eighth200x200.bmp");
  SPIFFS.remove("/chanceflurries.bmp");
  SPIFFS.remove("/betty_1.bmp");
  SPIFFS.remove("/betty_4.bmp");
  SPIFFS.remove("/marilyn_240x240x8.bmp");
  SPIFFS.remove("/miniwoof.bmp");
  SPIFFS.remove("/test.bmp");
  SPIFFS.remove("/tiger.bmp");
  SPIFFS.remove("/tiger_178x160x4.bmp");
  SPIFFS.remove("/tiger_240x317x4.bmp");
  SPIFFS.remove("/tiger_320x200x24.bmp");
  SPIFFS.remove("/tiger16T.bmp");
  SPIFFS.remove("/woof.bmp");

  SPIFFS.remove("/output5.bmp");
  SPIFFS.remove("/output6.bmp");
  SPIFFS.remove("/tractor_1.bmp");
  SPIFFS.remove("/tractor_4.bmp");
  SPIFFS.remove("/tractor_11.bmp");
  SPIFFS.remove("/tractor_44.bmp");
  SPIFFS.remove("/tractor_88.bmp");
}
#else
void deleteFiles()
{
  LittleFS.remove("logo200x200.bmp");
  LittleFS.remove("first200x200.bmp");
  LittleFS.remove("second200x200.bmp");
  LittleFS.remove("third200x200.bmp");
  LittleFS.remove("fourth200x200.bmp");
  LittleFS.remove("fifth200x200.bmp");
  LittleFS.remove("sixth200x200.bmp");
  LittleFS.remove("seventh200x200.bmp");
  LittleFS.remove("eighth200x200.bmp");
  LittleFS.remove("chanceflurries.bmp");
  LittleFS.remove("betty_1.bmp");
  LittleFS.remove("betty_4.bmp");
  LittleFS.remove("marilyn_240x240x8.bmp");
  LittleFS.remove("miniwoof.bmp");
  LittleFS.remove("test.bmp");
  LittleFS.remove("tiger.bmp");
  LittleFS.remove("tiger_178x160x4.bmp");
  LittleFS.remove("tiger_240x317x4.bmp");
  LittleFS.remove("tiger_320x200x24.bmp");
  LittleFS.remove("tiger16T.bmp");
  LittleFS.remove("woof.bmp");

  LittleFS.remove("output5.bmp");
  LittleFS.remove("output6.bmp");
  LittleFS.remove("tractor_1.bmp");
  LittleFS.remove("tractor_4.bmp");
  LittleFS.remove("tractor_11.bmp");
  LittleFS.remove("tractor_44.bmp");
  LittleFS.remove("tractor_88.bmp");
}
#endif

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
               "User-Agent: GxEPD_HD_Spiffs_Loader\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");
  bool ok = false;
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
  size_t written = 0;
#if defined(ESP32)
  fs::File file = SPIFFS.open(String("/") + target, "w+");
#else
  fs::File file = LittleFS.open(target, "w+");
#endif
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
      written += file.write(buffer, got);
      total += got;
    }
    delay(1); // yield();
  }
  file.close();
  Serial.print("done, "); Serial.print(total); Serial.print(" bytes transferred, written "); Serial.println(written);
}

void downloadFile_HTTPS(const char* host, const char* path, const char* filename, const char* fingerprint, const char* target, const char* certificate)
{
  // Use WiFiClientSecure class to create TLS connection
#if defined (ESP8266)
  BearSSL::WiFiClientSecure client;
  BearSSL::X509List cert(certificate ? certificate : certificate_rawcontent);
#else
  WiFiClientSecure client;
#endif
  Serial.println(); Serial.print("downloading file \""); Serial.print(filename);  Serial.println("\"");
  Serial.print("connecting to "); Serial.println(host);
#if defined (ESP8266)
  if (certificate) client.setTrustAnchors(&cert);
  else if (fingerprint) client.setFingerprint(fingerprint);
  else client.setInsecure();
#elif defined (ESP32)
  if (certificate) client.setCACert(certificate);
#endif
  if (!client.connect(host, httpsPort))
  {
    Serial.println("connection failed");
    return;
  }
  Serial.print("requesting URL: ");
  Serial.println(String("https://") + host + path + filename);
  client.print(String("GET ") + path + filename + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: GxEPD2_Spiffs_Loader\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");
  bool ok = false;
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
  size_t written = 0;
#if defined(ESP32)
  fs::File file = SPIFFS.open(String("/") + target, "w+");
#else
  fs::File file = LittleFS.open(target, "w+");
#endif
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
      written += file.write(buffer, got);
      total += got;
    }
    delay(1); // yield();
    if (total > 30000) delay(250); // helps for long downloads
  }
  file.close();
  Serial.print("done, "); Serial.print(total); Serial.print(" bytes transferred, written "); Serial.println(written);
}

// Set time via NTP, as required for x.509 validation
void setClock()
{
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}
