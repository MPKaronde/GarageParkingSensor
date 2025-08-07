#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_VL53L0X.h>

// Pin where the NeoPixel data line is connected
#define LED_PIN    18      // Use GPIO 5

// Number of LEDs in the grid (e.g. 8x8 = 64)
#define NUM_LEDS   32

// Create NeoPixel object
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Create Distance Sensor object
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// Sensor ranges
int GREEN_LIMIT = 750;
int YELLOW_LIMIT = 350;
int STOOOPPPP = 150;

void setup() {
  strip.begin();           // Initialize the NeoPixel strip
  strip.show();            // Turn off all LEDs initially
  strip.setBrightness(50); // Optional: set brightness (0–255)
  Serial.begin(115200);

  // wait for the VL53L0X to be ready
  if (!lox.begin()) {
    Serial.println("Failed to find VL53L0X chip");
    while (1);
  }
}

// take a reading from the sensor
int distance_reading()
{
  // read distance from sensor
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  int distance = measure.RangeMilliMeter;
  if (measure.RangeStatus == 4) { // make sure reading in range
    distance = -1;
  }

  return distance;
}

bool break_time(int limit)
{
  int distance = distance_reading();
  if(distance <= limit && distance != -1)
  {
    return true;
  }
  return false;
}

bool moving_car()
{
  int distance = distance_reading();
  if(distance >= GREEN_LIMIT)
  {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 255, 0)); // Yellow
      strip.show();
    }
  }
  if(distance < GREEN_LIMIT && distance >= YELLOW_LIMIT)
  {
     for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(220, 180, 24)); // Yellow
      strip.show();
    }
  }
  if(distance < YELLOW_LIMIT && distance >= STOOOPPPP)
  {
     for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(255, 0, 0)); // Red
      strip.show();
    }
  }
  if(distance <= STOOOPPPP && distance != -1)
  {
    for(int x = 0; x < 10; x++)
    {
      for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(255, 0, 0)); // Flashing red
        strip.show();
      }
      delay(50);
      for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0)); // off
        strip.show();
      }
      delay(50);
    }
    return false;
  }
  if(distance == -1)
  {
    return false;
  }
  return true;
}

bool readings_differ(int readingA, int readingB)
{
  if(readingA > 1500 && readingB > 1500)
  {
    return false;
  }
  if(readingA > 1500 && readingB == -1)
  {
    return false;
  }
  if(readingA == -1 && readingB > 1500)
  {
    return false;
  }

  int error = readingB - readingA;
  if(error < 0)
  {
    error = -error;
  }
  return error > 50;
}

void loop() {
  // wait for movement
  int readingA = distance_reading();
  delay(50);
  int readingB = distance_reading();

  // make a non error prone method
  if(readings_differ(readingA, readingB))
  {
    for(int i = 0; i < 150; i++)
    {
      if(!moving_car())
      {
        break;
      }
    }
  }
  else
  {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 255)); // blue
      strip.show();
    }
  }
}

/*
 // no object in range
  if(distance == -1 || distance > 1000)
  {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 255)); // Blue
      strip.show();
    }
  }

*/
