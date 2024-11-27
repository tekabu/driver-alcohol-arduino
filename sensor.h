#define SENSOR_PIN 34

unsigned long sensor_last_millis = 0;
int sensor_value = 0;

void updateSensorValue() {
  if (millis() - sensor_last_millis >= 2000) {
    sensor_value = analogRead(SENSOR_PIN);
    sensor_last_millis = millis();
  }
}
