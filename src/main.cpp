#include <Adafruit_LSM303_Accel.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_LIS2MDL.h>
#include <RTClib.h>
#include <SPI.h>
#include <SdFat.h>

#define IR_sensor_pin 0
#define temperature_sensor_pin 1 
#define phototransistor_pin 2
#define SD_card_chip_select_pin 4
#define SD_card_pin 10
#define status_light_pin 13

// Assign a unique ID to the accelerometer
Adafruit_LSM303_Accel_Unified accelerometer = Adafruit_LSM303_Accel_Unified(54321);

// Assign a unique ID to the magnetometer
Adafruit_LIS2MDL magnetometer = Adafruit_LIS2MDL(12345);

// Define the real time clock
RTC_DS3231 real_time_clock;

File log_file;

int last_flushed_second = 0;

struct data_container{
    float temperature_reading;
    uint16_t phototransistor_reading;
    uint16_t IR_sensor_reading;
    uint16_t seconds_now;
    unsigned long milliseconds;
    float accelerometer_x;
    float accelerometer_y;
    float accelerometer_z;
    float magnetometer_x;
    float magnetometer_y;
    float magnetometer_z;
} data;

const int buffer_size = 15;
data_container data_buffer[buffer_size];
int buffer_index = 0; 

SdFat SD;

void wait_for_startup(void) {

  if (!accelerometer.begin() || !magnetometer.begin() || !real_time_clock.begin() || !SD.begin(SD_card_chip_select_pin, SPI_FULL_SPEED)) {
    while (true);
  }
}

void flash_good_signal(void) {

  if (data.seconds_now % 2 == 0) {
    digitalWrite(status_light_pin, HIGH);
  } else {
    digitalWrite(status_light_pin, LOW);
  }
}

void set_pin_modes(void) {

  pinMode(temperature_sensor_pin, INPUT);
  pinMode(phototransistor_pin, INPUT);
  pinMode(IR_sensor_pin, INPUT);

  pinMode(SD_card_pin, OUTPUT);

}

void print_to_log(void) {

  data_buffer[buffer_index] = data;
  buffer_index++;

  if (buffer_index >= buffer_size) {

    int bytes_written = log_file.write((const uint8_t *)&data_buffer, sizeof(data_buffer));

    if (bytes_written > 0){
      flash_good_signal();
    }
    else{
      digitalWrite(status_light_pin, LOW);
    }

    buffer_index = 0;
  }

  if (abs(data.seconds_now - last_flushed_second) >= 30) {
      log_file.flush();
      last_flushed_second = data.seconds_now;
  }


}


void read_sensors(void) {

  sensors_event_t accelerometer_event, magnetometer_event;

  accelerometer.getEvent(&accelerometer_event);

  data.accelerometer_x = accelerometer_event.acceleration.x;
  data.accelerometer_y = accelerometer_event.acceleration.y;
  data.accelerometer_z = accelerometer_event.acceleration.z;

  magnetometer.getEvent(&magnetometer_event);

  data.magnetometer_x = magnetometer_event.magnetic.x;
  data.magnetometer_y = magnetometer_event.magnetic.y;
  data.magnetometer_z = magnetometer_event.magnetic.z;

  data.temperature_reading = (analogRead(temperature_sensor_pin) * (3.3 / 1024) - 0.5) * 100;

  data.phototransistor_reading = analogRead(phototransistor_pin);

  data.IR_sensor_reading = analogRead(IR_sensor_pin);

  data.seconds_now = real_time_clock.now().second();

  data.milliseconds = millis();

}

void setup(void) {

  wait_for_startup();

  set_pin_modes();

  accelerometer.setMode(LSM303_MODE_NORMAL);

  log_file = SD.open("log.dat", O_CREAT | O_WRITE);

}

void loop(void) {

  read_sensors();

  print_to_log();

}
