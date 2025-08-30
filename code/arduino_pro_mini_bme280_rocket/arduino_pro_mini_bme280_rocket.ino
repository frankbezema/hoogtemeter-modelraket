// model rocket hoogte meter, toon max alt op een oled
// copyright Frank Bezema 2022

#include <Wire.h>
#include "SparkFunBME280.h"
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DEFAULT_MEASUR_MILLIS 1000 /* Get sensor time by default (ms)*/

float SEALEVELPRESSURE_HPA;
float hoogte, heightfloat_decimeters, timems, reading;
float localAltitude = 0;

int maxhoogte;
BME280 mySensor;

unsigned long delayTime = 89;  // millisecs between measurements
const int COUNT = 10;          // number of sensor readings to avarage, must be > 3

// callback function for doing comparisons
template<typename T>
int myCompareFunction(const void *arg1, const void *arg2) {
  T *a = (T *)arg1;  // cast to pointers to type T
  T *b = (T *)arg2;
  // a less than b?
  if (*a < *b)
    return -1;
  // a greater than b?
  if (*a > *b)
    return 1;
  // must be equal
  return 0;
}  // end of myCompareFunction

void setup() {
  // Start serial communication for debugging purposes
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  bool status;
  Wire.begin();

  delay(3);
  // BME280 needs lots of strange delays and/or double reads to start correctly?
  mySensor.setI2CAddress(0x76);
  delay(20);
  if (mySensor.beginI2C() == false) Serial.println("Sensor connect failed");
  delay(20);
  mySensor.setPressureOverSample(4);
  mySensor.setTempOverSample(4);
  delay(20);
  mySensor.readTempC();
  delay(20);
  localAltitude = mySensor.readFloatAltitudeMeters();  //Set reference altitude
  delay(20);
  localAltitude = mySensor.readFloatAltitudeMeters();  //Set reference altitude
  delay(20);
  Serial.println();
}

void loop() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000);  // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  int tijdtienden;
  int hoogte;
  float reading, sum = 0;
  float heightfloatmeters = 0.0;

  localAltitude = mySensor.readFloatAltitudeMeters();

  timems = millis();
  mySensor.readTempC();
  delay(10);
  timems = timems / 100;  // time tienden van seconden
  tijdtienden = (int)timems;
  // take COUNT readings of AltitudeMeters
  // total time 80ms
  float readings[10];
  sum = 0.0;
  for (int i = 0; i < COUNT; i++) {
    readings[i] = mySensor.readFloatAltitudeMeters() - localAltitude;
    if (readings[i] < 0.0) readings[i] = 0.0;
    delay(10);
    sum = sum + readings[i];
  }
  qsort(readings, COUNT, sizeof(float), myCompareFunction<float>);
  sum = 0.0;
  for (int i = 1; i < COUNT - 3; i++) {
    sum = sum + readings[i];
    //Serial.println (readings[i]);
  }
  reading = sum / (COUNT - 4);
  if (reading < 0.0) reading = 0.0;
  //Serial.print ("hoogte[m]:");
  //Serial.println (reading);
  heightfloatmeters = reading;
  if (heightfloatmeters < 0.0) heightfloatmeters = 0.0001;
  hoogte = (int)heightfloatmeters;
  if (hoogte > maxhoogte) maxhoogte = hoogte;
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("maxhoogte:");
  display.println(maxhoogte);

  display.display();
  delay(delayTime);

  die();
}

void die(void) {
  wdt_disable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  while (true) {
    sleep_enable();
    cli();
    sleep_cpu();
  }
}
