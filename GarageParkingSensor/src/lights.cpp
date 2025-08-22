/*
class to wrap the functionality of the signal light strip
*/

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class lightStrip
{
    public:

        // Flashing delays in milliseconds
        const int slow_flash_delay = 250;
        const int moderate_flash_delay = 100;
        const int fast_flash_delay = 50;
        int MAX_DISTANCE = 800; // max distance to show on bar graph

        int blue[3] = {0, 0, 255};
        int green[3] = {0, 255, 0};
        int yellow[3] = {250, 240, 94};
        int red[3] = {255, 0, 0};

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


        void color_move(int color[3], int delay_ms, int tail_size)
        {
            int center_high = NUM_LEDS / 2;
            int center_low = center_high - 1;

            for(int i = 0; i < center_high; i++)
            {
                int tail = i - tail_size;
                if(tail < 0)
                {
                    tail = NUM_LEDS - tail;
                }

                strip.setPixelColor(center_low - i, strip.Color(color[0], color[1], color[2]));
                strip.setPixelColor(center_high + i, strip.Color(color[0], color[1], color[2]));

                strip.setPixelColor(center_low - tail, strip.Color(0, 0, 0));
                strip.setPixelColor(center_high + tail, strip.Color(0, 0, 0));
                strip.show();

                delay(delay_ms);
            }

            // 🧹 Clean up: turn off remaining tail pixels at the end
            for (int i = center_high - tail_size; i < center_high; i++)
            {
                if (i >= 0)
                {
                    strip.setPixelColor(center_low - i, strip.Color(0, 0, 0));
                    strip.setPixelColor(center_high + i, strip.Color(0, 0, 0));
                }
            }
            strip.show();

        }

        // moderately flashing yellow for car approaching
        void slow_flash_yellow()
        {
            for (int i = 0; i < NUM_LEDS; i++) {
                strip.setPixelColor(i, strip.Color(250, 240, 94)); // yellow
            }
            strip.show();
            delay(slow_flash_delay);
            for (int i = 0; i < NUM_LEDS; i++) {
                strip.setPixelColor(i, strip.Color(0, 0, 0)); // off
            }
            strip.show();
            delay(slow_flash_delay);
        }

        // moderate flashing red for good car position
        void moderate_flash_red()
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
};