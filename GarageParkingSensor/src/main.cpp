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

/*
TODO:
-include lighting animations when on approach
-make distance lights detect backing out as well
-error handling to determine if bad data is being returned from sensor
*/

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

// when we believe the car is moving
// returns false if no longer believes car is moving (out of range or passed STOOOOPPPP)
bool moving_car()
{
  int distance = distance_reading();

  // safe distance
  if(distance >= GREEN_LIMIT)
  {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 255, 0)); // Green
      strip.show();
    }
  }

  // slow down distance
  if(distance < GREEN_LIMIT && distance >= YELLOW_LIMIT)
  {
     for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(220, 180, 24)); // Yellow
      strip.show();
    }
  }

  // get ready to stop distance
  if(distance < YELLOW_LIMIT && distance >= STOOOPPPP)
  {
     for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(255, 0, 0)); // Red
      strip.show();
    }
  }

  // STOOOOPPP!!!!!
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
    // assume the car has stopped
    return false;
  }

  // out of range, assume there is no car
  if(distance == -1)
  {
    return false;
  }

  // assume car is still moving
  return true;
}

// check if two readings are far apart enough to trigger moving car
bool readings_differ(int readingA, int readingB)
{
  // special cases to check for very far vs out of range readings
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

  // check if readings are within range
  int error = readingB - readingA;
  if(error < 0)
  {
    error = -error;
  }
  return error > 50;
}

void loop() {
  // check if movement is occuring
  int readingA = distance_reading();
  delay(50);
  int readingB = distance_reading();
  if(readings_differ(readingA, readingB))
  {
    // assume car is still moving for time to complete loop or until otherwise signaled
    for(int i = 0; i < 150; i++)
    {
      if(!moving_car())
      {
        break;
      }
    }
  }

  // blue color = standby mode
  else
  {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 255)); // blue
      strip.show();
    }
  }
}
