#include <Adafruit_LSM303_Accel.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_LIS2MDL.h>
#include <RTClib.h>
#include <SD.h>

#define IR_sensor_pin 0
#define temperature_sensor_pin 1 
#define phototransistor_pin 2
#define SD_card_chip_select_pin 4
#define SD_card_pin 10

// Assign a unique ID to the accelerometer
Adafruit_LSM303_Accel_Unified accelerometer = Adafruit_LSM303_Accel_Unified(54321);

// Assign a unique ID to the magnetometer
Adafruit_LIS2MDL magnetometer = Adafruit_LIS2MDL(12345);

// Define the real time clock
RTC_DS3231 real_time_clock;

File log_file;

sensors_event_t accelerometer_event, magnetometer_event;

float temperature_reading;
float phototransistor_reading;
float IR_sensor_reading;

float seconds_now;
DateTime now;

void wait_for_startup(void) {

  while (!Serial); 

  if (!accelerometer.begin()) {
    Serial.println("Ooops, accelerometer failed");
    while (true);
  }

  if(!magnetometer.begin()) {
    Serial.println("Ooops, magnetometer failed.");
    while (true);
  }

  if (!real_time_clock.begin()) {
      Serial.println("Oops, real time clock failed.");
      while(true);
  }

  if (!SD.begin(SD_card_chip_select_pin)) {
    Serial.println("Oops, SD card failed");
    while(1);
  }


}

void set_pin_modes(void) {

  pinMode(temperature_sensor_pin, INPUT);
  pinMode(phototransistor_pin, INPUT);
  pinMode(IR_sensor_pin, INPUT);

  pinMode(SD_card_pin, OUTPUT);

}

void print_to_serial(void) {

  Serial.print("X: ");
  Serial.print(accelerometer_event.acceleration.x);
  Serial.print("  ");
  Serial.print("Y: ");
  Serial.print(accelerometer_event.acceleration.y);
  Serial.print("  ");
  Serial.print("Z: ");
  Serial.print(accelerometer_event.acceleration.z);
  Serial.print("  ");
  Serial.println("m/s^2");

  Serial.print("X: ");
  Serial.print(magnetometer_event.magnetic.x);
  Serial.print("  ");
  Serial.print("Y: ");
  Serial.print(magnetometer_event.magnetic.y);
  Serial.print("  ");
  Serial.print("Z: ");
  Serial.print(magnetometer_event.magnetic.z);
  Serial.print("  ");
  Serial.println("uT");

  Serial.println("Temperature = " + String(temperature_reading) + " C");
  Serial.println("Phototransistor = " + String(phototransistor_reading));
  Serial.println("IR Sensor = " + String(IR_sensor_reading));
  Serial.println("Seconds = " + String(seconds_now));

}

void print_to_log(void) {

  log_file.print("X: ");
  log_file.print(accelerometer_event.acceleration.x);
  log_file.print("  ");
  log_file.print("Y: ");
  log_file.print(accelerometer_event.acceleration.y);
  log_file.print("  ");
  log_file.print("Z: ");
  log_file.print(accelerometer_event.acceleration.z);
  log_file.print("  ");
  log_file.println("m/s^2");
  log_file.print("X: ");
  log_file.print(magnetometer_event.magnetic.x);
  log_file.print("  ");
  log_file.print("Y: ");
  log_file.print(magnetometer_event.magnetic.y);
  log_file.print("  ");
  log_file.print("Z: ");
  log_file.print(magnetometer_event.magnetic.z);
  log_file.print("  ");
  log_file.println("uT");
  log_file.println("Temperature = " + String(temperature_reading) + " C");
  log_file.println("Phototransistor = " + String(phototransistor_reading));
  log_file.println("IR Sensor = " + String(IR_sensor_reading));
  log_file.println("Seconds = " + String(seconds_now));

  log_file.flush();

}

void read_sensors(void) {

  accelerometer.getEvent(&accelerometer_event);
  magnetometer.getEvent(&magnetometer_event);

  temperature_reading = analogRead(temperature_sensor_pin);

  temperature_reading *= 3.3;
  temperature_reading /= 1024;
  temperature_reading -= 0.5;
  temperature_reading *= 100;

  phototransistor_reading = analogRead(phototransistor_pin);

  IR_sensor_reading = analogRead(IR_sensor_pin);

  now = real_time_clock.now();
  seconds_now = now.second();

}

void setup(void) {

  Serial.begin(9600);

  wait_for_startup();

  set_pin_modes();

  accelerometer.setMode(LSM303_MODE_NORMAL);

  log_file = SD.open("log.csv", FILE_WRITE);

}

void loop(void) {

  read_sensors();

  print_to_serial();

  print_to_log();
  
  //delay(500);
}
