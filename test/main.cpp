#include <Wire.h>
#include "ADS1X15.h"
 
ADS1115 ADS(0x48);

struct data_container{
    float IR_sensor_reading;
    unsigned long milliseconds;
} data;


unsigned long starttime = 0;

float multiplier = 0;

void read_sensors(void) {

  data.milliseconds = millis() - starttime;

  data.IR_sensor_reading = ADS.readADC_Differential_0_1() * multiplier;

}

void print_data(void) {

  Serial.write(0xAA); 

  Serial.write((uint8_t*)&data, sizeof(data));

}

void setup(void) {

  Wire.begin();               
  Wire.setClock(400000L); 

  ADS.begin();

  ADS.setGain(16);

  ADS.setDataRate(7);

  multiplier = ADS.toVoltage();

  Serial.begin(115200);

  starttime = millis();

}


void loop(void) {

  read_sensors();

  print_data();

}
