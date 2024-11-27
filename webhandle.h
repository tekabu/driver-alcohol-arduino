#include <html.h>

String processHome() {
  String html = home_html;
  return html;
}

String processContacts() {
  String html = contacts_html;
  html.replace("%MOBILE1%", mobile1);
  html.replace("%MOBILE2%", mobile2);
  html.replace("%MOBILE3%", mobile3);
  html.replace("%MOBILE4%", mobile4);
  html.replace("%MOBILE5%", mobile5);
  return html;
}

void handleHome() {
  String htmlContent = processHome();
  server.send(200, "text/html", htmlContent);
}

void handleSensor() {
  String val = String(sensor_value);
  server.send(200, "text/plain", val);
}

void handleLatlng() {
  String val = clat;
  val += ", ";
  val += clng;
  server.send(200, "text/plain", val);
}

void handleContacts() {
  String htmlContent = processContacts();
  server.send(200, "text/html", htmlContent);
}

void handleFormSubmit() {
  mobile1 = "";
  mobile2 = "";
  mobile3 = "";
  mobile4 = "";
  mobile5 = "";

  if (server.hasArg("mobile1")) {
    mobile1 = server.arg("mobile1");
    Serial.print(F("Received mobile1: "));
    Serial.println(mobile1);
  }
  if (server.hasArg("mobile2")) {
    mobile2 = server.arg("mobile2");
    Serial.print(F("Received mobile2: "));
    Serial.println(mobile2);
  }
  if (server.hasArg("mobile3")) {
    mobile3 = server.arg("mobile3");
    Serial.print(F("Received mobile3: "));
    Serial.println(mobile3);
  }
  if (server.hasArg("mobile4")) {
    mobile4 = server.arg("mobile4");
    Serial.print(F("Received mobile4: "));
    Serial.println(mobile4);
  }
  if (server.hasArg("mobile5")) {
    mobile5 = server.arg("mobile5");
    Serial.print(F("Received mobile5: "));
    Serial.println(mobile5);
  }
  saveContacts();
  server.sendHeader("Location", "/contacts");
  server.send(303);
}
