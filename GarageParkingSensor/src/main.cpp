#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// Pin where the NeoPixel data line is connected
#define LED_PIN    18      // Use GPIO 5

// Number of LEDs in the grid (e.g. 8x8 = 64)
#define NUM_LEDS   32

// Create NeoPixel object
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();           // Initialize the NeoPixel strip
  strip.show();            // Turn off all LEDs initially
  strip.setBrightness(50); // Optional: set brightness (0–255)
}

void loop() {
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
}

