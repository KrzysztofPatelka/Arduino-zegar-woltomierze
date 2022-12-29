#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"

Adafruit_BME280 bme;
RTC_DS1307 RTC;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int HOUR_PIN=5;
const int MIN_PIN=6;
DateTime now;
int tempMin = -1;

void setup() {
  pinMode(HOUR_PIN,OUTPUT);
  pinMode(MIN_PIN,OUTPUT);
  bme.begin(0x76);
  bme.setSampling(Adafruit_BME280::MODE_FORCED,
                  Adafruit_BME280::SAMPLING_X1, // temperature
                  Adafruit_BME280::SAMPLING_X1, // pressure
                  Adafruit_BME280::SAMPLING_X1, // humidity
                  Adafruit_BME280::FILTER_OFF);
  Wire.begin();
  RTC.begin();
  RTC.isrunning();
  //RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(500);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(15, 28);
  display.print("START...");
  display.display();
  delay(5000);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.display();
}

void loop()
{
  now = RTC.now();
  if (tempMin != now.minute())
  {
    tempMin = now.minute();
    bme.takeForcedMeasurement();
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("T: ");
    display.print(bme.readTemperature(), 1.F);
    display.println(" st. C");
    display.println("");
    display.print("P: ");
    display.print(bme.readPressure() / 100.0F, 0.F);
    display.println(" hPa");
    display.println("");
    display.print("H: ");
    display.print(bme.readHumidity(), 0.F);
    display.println(" %");
    display.println("");
    display.println("");
    display.print("   " + now.timestamp().substring(0, 10) + " " + now.timestamp().substring(11, 16));
    display.display();
    analogWrite(HOUR_PIN,map(now.twelveHour(),0,20,0,255));
    analogWrite(MIN_PIN,map(now.minute(),0,60,0,255));
  }
}