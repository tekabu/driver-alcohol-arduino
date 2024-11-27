#include <WiFi.h>
#include <WebServer.h>

#define BAUD 9600
#define RXD2 16
#define TXD2 17

WebServer server(80);
HardwareSerial SoftSerial(2);

const char* ssid = "ESP32-AP";
const char* password = "123456789";
String mobile1, mobile2, mobile3, mobile4, mobile5;
unsigned long mobile1_prevtime;
unsigned long mobile2_prevtime;
unsigned long mobile3_prevtime;
unsigned long mobile4_prevtime;
unsigned long mobile5_prevtime;

String at_resp;
String _OK = "OK" + String(char(13)) + String(char(10));
String clat, clng;
unsigned long prevtime = 0;

#include <sensor.h>
#include <flashdata.h>
#include <webhandle.h>

void setup() {
  // Serial.begin(BAUD);
  SoftSerial.begin(BAUD, SERIAL_8N1, RXD2, TXD2);

  loadContacts();

  WiFi.softAP(ssid, password);
  Serial.println(F("Access Point Started"));
  Serial.print(F("IP Address: "));
  Serial.println(WiFi.softAPIP());

  server.on("/", handleHome);
  server.on("/sensor", HTTP_GET, handleSensor);
  server.on("/latlng", HTTP_GET, handleLatlng);
  server.on("/contacts", handleContacts);
  server.on("/submit", handleFormSubmit);

  server.begin();
  Serial.println(F("Server started"));
}

void loop() {
  server.handleClient();
  updateSensorValue();

  if (millis() - prevtime >= 5000) {
    if (checkAT()) {
      if (enableGPS()) {
        if (checkGPS()) {
          readGPS();
        }
      }
    }
    prevtime = millis();
  }

  if (sensor_value >= 4095) {
    Serial.println("alcohol detected...");
    if (sendsms(mobile1, mobile1_prevtime)) {
      mobile1_prevtime = millis();
    }
    if (sendsms(mobile2, mobile2_prevtime)) {
      mobile2_prevtime = millis();
    }
    if (sendsms(mobile3, mobile3_prevtime)) {
      mobile3_prevtime = millis();
    }
    if (sendsms(mobile4, mobile4_prevtime)) {
      mobile4_prevtime = millis();
    }
    if (sendsms(mobile5, mobile5_prevtime)) {
      mobile5_prevtime = millis();
    }
  }
}

bool sendsms(String mobile, unsigned long prevtime) {
  if (clat.length() == 0) {
    return false;
  }
  if (mobile.length() == 0) {
    return false;
  }
  if (millis() - prevtime >= (1000 * 60) * 3 || prevtime == 0) {
    if (at_command("AT+CMGF=1\r", _OK, 1000)) {
      Serial.println("sending sms...");
      String cmd = "AT+CMGS=\"@MOBILE\"";
      cmd.replace("@MOBILE", mobile);
      SoftSerial.println(cmd);
      delay(1000);
      cmd = clat;
      cmd += ",";
      cmd += clng;
      SoftSerial.println(cmd);
      delay(1000);
      SoftSerial.write(26);
      delay(5000);
      log("sms sent...");
      prevtime = millis();
      return true;
    }
  }
  return false;
}

bool checkAT() {
  bool at = at_command("AT\r", _OK, 1000);
  if (!at) return false;
  delay(1000);
  return true;
}

bool checkGPS() {
  unsigned long ctime = millis();
  bool at = false;
  while (1) {
    if (millis() - ctime >= 30000) break;
    at = at_command("AT+LOCATION=2\r", _OK, 5000);
    if (at) break;
  }
  delay(1000);
  return at;
}

bool getSignal() {
  bool at = at_command("AT+CSQ\r", _OK, 1000);
  if (!at) return false;
  delay(1000);
  return true;
}

bool enableGPS() {
  bool at = at_command("AT+GPS=1\r", _OK, 1000);
  if (!at) return false;
  delay(1000);
  return true;
}

void runUntilOK(String command, String wait_for, long timeout) {
  while (1) {
    bool at = at_command(command, wait_for, timeout);
    delay(1000);
    if (at) break;
  }
}

bool readGPS() {
  if (!at_command("AT+LOCATION=2\r", _OK, 1000)) {
    return false;
  }
  int r1Index = at_resp.indexOf('\r');
  int cIndex = at_resp.indexOf(',', r1Index + 1);
  int r2Index = at_resp.indexOf('\r', cIndex + 1);

  clat = at_resp.substring(r1Index, cIndex);
  clng = at_resp.substring(cIndex + 1, r2Index);

  clat.trim();
  clng.trim();

  Serial.print(clat);
  Serial.print(", ");
  Serial.print(clng);
  Serial.print(", ");
  Serial.print(clat.toFloat());
  Serial.print(", ");
  Serial.println(clng.toFloat());

  if (clat.toFloat() <= 0.0 || clng.toFloat() <= 0.0) return false;

  return true;
}

bool at_command(String command, String wait_for, long timeout) {
  Serial.print(command);
  SoftSerial.print(command);
  unsigned long ptime = millis();
  bool ret = false;
  String res;
  at_resp = "";
  while (true) {
    if (ret) break;
    if (millis() - ptime >= timeout) break;
    while (SoftSerial.available()) {
      char c = SoftSerial.read();
      res += String(c);
      if (res.endsWith(wait_for)) {
        ret = true;
        at_resp = res;
      }
    }
  }
  command.trim();
  Serial.print(command);
  Serial.print(": ");
  Serial.println(ret ? "success" : "failed");
  log(res);
  flush_softser();
  return ret;
}

void flush_softser() {
  while (SoftSerial.available()) {
    Serial.write(SoftSerial.read());
    delay(10);
  }
  Serial.println();
}

void a9_softreset() {
  Serial.println("A9 Soft Reset...");
  at_command("AT+RST=1\r", _OK, 1000);
  delay(15000);
}

void log(String s) {
  Serial.println("----------------------------------------------------");
  Serial.println(s);
  Serial.println("----------------------------------------------------");
}
