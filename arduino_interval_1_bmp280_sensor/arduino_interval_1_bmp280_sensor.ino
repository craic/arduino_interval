/* arduino_interval_1_bmp280_sensor 
 *  
 *  Example code that reads the temperature from an Adafruit BME289 breakout board
 *  
 *  This is a simplified version of the Adafruit code described below - the code
 *  for humidity, pressue and altitude has been removed for clarity
 *  
 *  Rob Jones 2017  jones@craic.com
 *  
 */


/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

// Includes and Definitions for the BME280 sensor

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

Adafruit_BME280 bme; // I2C

const int onboardLedPin = 13;

unsigned long prevMilliSec = 0;
unsigned long intervalMilliSec = 10000;

void sensorSetup() {
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    flashOnboardLed();
//    while (1);
// flash the onboard LED ??
  }
}


String sensorRead() {
  float temperature = bme.readTemperature() * 9.0/5.0 + 32.0;
  String sensorString = String(temperature, 3);
  return(sensorString);
}

void setup() {

  pinMode(onboardLedPin, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Temperature");

  sensorSetup();
}


// Flash the onboard LED to signal an error
void flashOnboardLed() {
  while(1) {
    digitalWrite(onboardLedPin, HIGH);
    delay(200);
    digitalWrite(onboardLedPin, LOW);
    delay(200);
  }
}


void loop() {
  
    if(millis() - prevMilliSec >= intervalMilliSec ) {
      Serial.println(sensorRead());
      prevMilliSec = millis();    
    }
}


