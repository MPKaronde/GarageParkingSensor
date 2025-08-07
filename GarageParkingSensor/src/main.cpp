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

void loop() {

  int distance = distance_reading();
  if(distance == -1)
  {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(255, 0, 0)); // Red
      strip.show();
      delay(50);
    }
  }
  else 
  {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 255, 0)); // Green
      strip.show();
      delay(50);
    }
  }


}
/*
  // Simple color cycle
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0)); // Red
    strip.show();
    delay(50);
  }

  delay(500);

  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 255, 0)); // Green
    strip.show();
    delay(50);
  }

  delay(500);

  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 255)); // Blue
    strip.show();
    delay(50);
  }

  delay(500);

*/
