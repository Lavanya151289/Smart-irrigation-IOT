#include <Wire.h>
#include <hd44780.h>                       
#include <hd44780ioClass/hd44780_I2Cexp.h> 
hd44780_I2Cexp lcd; 

#include "DHT.h"
#include <SPI.h>
#include <SD.h>

const int relay_Pin = 8;
const int DHT11_Sensor = 9;
const int moisture_sensor = A0;
const int rain_Sensor = A1;
const int chipSelect = 10;

#define DHTTYPE DHT11
int moisture_sensor_value;
int rain_sensor_value;
float humidity_value, temperature_value;
DHT dht(DHT11_Sensor, DHTTYPE);

void setup() {
  Serial.begin(9600);
  pinMode(relay_Pin, OUTPUT);
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("Smart Irrigation");
  lcd.setCursor(0, 1);
  lcd.print("SYSTEM");
  digitalWrite(relay_Pin, LOW);
  dht.begin();
  delay(2000);
}

void loop() {
  readDTH11_Sensor();
  moisture_level_detected();
  water_motor_start();
}

void readDTH11_Sensor() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  humidity_value = dht.readHumidity();
  // Read temperature as Celsius (the default)
  temperature_value = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity_value) || isnan(temperature_value)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  lcd.clear();
  lcd.print("Humidity %: ");
  lcd.setCursor(0, 1);
  lcd.print(humidity_value);
  Serial.print("Humidity: ");
  Serial.print(humidity_value);
  Serial.println("%");
  delay(2000);

  lcd.clear();
  lcd.print("Temperature degC:");
  lcd.setCursor(0, 1);
  lcd.print(temperature_value);
  Serial.print("Temperature: ");
  Serial.print(temperature_value);
  Serial.println("C");
  delay(2000);

  lcd.clear();
  lcd.print("Moisture Level:");
  lcd.setCursor(0, 1);
  lcd.print(moisture_sensor_value);
  Serial.print("Moisture Level: ");
  Serial.println(moisture_sensor_value);
  delay(2000);
}

void moisture_level_detected() {
  moisture_sensor_value = analogRead(moisture_sensor);
}

void water_motor_start() {
  rain_sensor_value = analogRead(rain_Sensor);
  Serial.print("Rain sensor value: ");
  Serial.println(rain_sensor_value);
  delay(1000);
  if (rain_sensor_value > 700) {
    if (moisture_sensor_value > 700 ) {
      digitalWrite(relay_Pin, LOW);
      lcd.clear();
      lcd.print("PUMP ON");
      Serial.println("PUMP ON");
      delay(2000);
    } else {
      digitalWrite(relay_Pin, HIGH);
      lcd.clear();
      lcd.print("PUMP OFF");
      Serial.println("PUMP OFF");
      delay(2000);
    }
  }
  else{

      digitalWrite(relay_Pin, HIGH);
      lcd.clear();
      lcd.print("PUMP OFF");
      Serial.println("PUMP OFF");
      delay(2000);
  }
}
