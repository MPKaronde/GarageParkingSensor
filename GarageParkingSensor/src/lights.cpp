/*
class to wrap the functionality of the signal light strip
*/

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class lightStrip
{
    public:
        // Constructor: initializes the NeoPixel strip with the given pin and number of LEDs
        lightStrip(int ledPin, int numLeds): 
        strip(numLeds, ledPin, NEO_GRB + NEO_KHZ800)
        {
            LED_PIN = ledPin;
            NUM_LEDS = numLeds;
            strip.begin();
            strip.show(); // Initialize all pixels to 'off'
        }

        // static blue for idle / no movement detected
        void static_blue()
        {
            for (int i = 0; i < NUM_LEDS; i++) {
                strip.setPixelColor(i, strip.Color(0, 0, 255)); // blue
            }
            strip.show();
        }

        // slow flashing blue for error detected
        void slow_flash_blue()
        {
            for (int i = 0; i < NUM_LEDS; i++) {
                strip.setPixelColor(i, strip.Color(0, 0, 255)); // blue
            }
            strip.show();
            delay(slow_flash_delay);
            for (int i = 0; i < NUM_LEDS; i++) {
                strip.setPixelColor(i, strip.Color(0, 0, 0)); // off
            }
            strip.show();
            delay(slow_flash_delay);
        }

        // slow flashing green for car safe distance
        void slow_flash_green()
        {
            for (int i = 0; i < NUM_LEDS; i++) {
                strip.setPixelColor(i, strip.Color(0, 255, 0)); // green
            }
            strip.show();
            delay(slow_flash_delay);
            for (int i = 0; i < NUM_LEDS; i++) {
                strip.setPixelColor(i, strip.Color(0, 0, 0)); // off
            }
            strip.show();
            delay(slow_flash_delay);
        }

        // moderately flashing yellow for car approaching
        void moderate_flash_yellow()
        {
            for (int i = 0; i < NUM_LEDS; i++) {
                strip.setPixelColor(i, strip.Color(255, 255, 0)); // yellow
            }
            strip.show();
            delay(moderate_flash_delay);
            for (int i = 0; i < NUM_LEDS; i++) {
                strip.setPixelColor(i, strip.Color(0, 0, 0)); // off
            }
            strip.show();
            delay(moderate_flash_delay);
        }

        // moderate flashing red for good car position
        void slow_flash_red()
        {
            for (int i = 0; i < NUM_LEDS; i++) {
                strip.setPixelColor(i, strip.Color(255, 0, 0)); // red
            }
            strip.show();
            delay(moderate_flash_delay);
            for (int i = 0; i < NUM_LEDS; i++) {
                strip.setPixelColor(i, strip.Color(0, 0, 0)); // off
            }
            strip.show();
            delay(moderate_flash_delay);
        }

        // fast flashing red for car too close
        void fast_flash_red()
        {
            for (int i = 0; i < NUM_LEDS; i++) {
                strip.setPixelColor(i, strip.Color(255, 0, 0)); // red
            }
            strip.show();
            delay(fast_flash_delay);
            for (int i = 0; i < NUM_LEDS; i++) {
                strip.setPixelColor(i, strip.Color(0, 0, 0)); // off
            }
            strip.show();
            delay(fast_flash_delay);
        }


    private:
        int LED_PIN; // pin where the NeoPixel data line is connected
        int NUM_LEDS; // number of LEDs in the strip
        Adafruit_NeoPixel strip; // NeoPixel object

        // Flashing delays in milliseconds
        const int slow_flash_delay = 250;
        const int moderate_flash_delay = 100;
        const int fast_flash_delay = 50;
};