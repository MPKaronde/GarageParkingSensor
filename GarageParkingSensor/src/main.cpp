// #include <Arduino.h>
// #include "lights.cpp"
// #include "sensors.cpp"

// // LED constants
// #define LED_PIN    18      
// #define NUM_LEDS   32

// // TOF sensor constants
// #define XSHUT_LEFT 14
// #define XSHUT_RIGHT 13

// // Ultrasonic sensor constants
// #define TRIG_PIN   4
// #define ECHO_PIN   34

// // hardware objects
// lightStrip strip(LED_PIN, NUM_LEDS);
// Sensors sensors(XSHUT_LEFT, XSHUT_RIGHT, TRIG_PIN, ECHO_PIN);

// // num times we will re check error readings
// const int ERROR_RECHECKS = 3;

// /*
// Distance limits for each color
// Each color is displayed w/ sensor readings in range (previous limit, current limit]
// w/ green capped at out of range on the upper end and fast red capped at 0 on the lower end
// */
// const int GREEN_LIMIT = 750;
// const int YELLOW_LIMIT = 350;
// const int RED_LIMIT = 150;

// // used to determine if car is moving
// int prev_reading = 0;
// const int MOVEMENT_THRESHOLD = 20; // difference in readings to consider movement

// // setup code
// void setup()
// {
//     Serial.begin(115200);
//     delay(1000); // Allow time for serial to initialize
// }

// // determine if the car is moving based on difference between prev and current reading
// // return true if moving, false if not
// bool car_moving(int current_reading)
// {

//     int old_prev = prev_reading; // save old previous reading
//     prev_reading = current_reading; // update previous reading

//     // out of range special cases
//     if(old_prev == -1 && current_reading != -1)
//     {
//         return true;
//     }
//     if (current_reading == -1 && old_prev != -1)
//     {
//         return true;
//     }
//     if(old_prev == -1 && current_reading == -1)
//     {
//         return false;
//     }
   

//     // check if the car is moving based on the difference from the previous reading
//     if (abs(current_reading - old_prev) > MOVEMENT_THRESHOLD) {
//         return true;
//     }
//     return false;
// }

// void loop()
// {
//     // get current reading
//     int reading = sensors.get_reading();

//     /*
//     Look for consistent error (as opposed to occasional sensor glitches) and signal error if needed
//     */
//     int i = 0;
//     while(reading == -2 && i < ERROR_RECHECKS) // recheck reading if error
//     {
//         delay(250); // wait before rechecking
//         reading = sensors.get_reading();
//         i++;
//     }
//     if(reading == -2)
//     {
//         strip.slow_flash_blue(); // indicate error with slow flashing blue
//         Serial.println("Error reading sensors, please check connections.");
//         return; // exit if still error after rechecks
//     }
    
//     // print reading to serial for debugging
//     Serial.println("Decided reading: " + String(reading));

//     // determine if car is moving
//     bool moving = car_moving(reading);

//     // decide appopriate light pattern based on reading and movement
//     if(!moving)
//     {
//         strip.static_blue();
//         return;
//     }
//     if(reading >= GREEN_LIMIT || reading == -1)
//     {
//         strip.slow_flash_green();
//         return;
//     }
//     if(reading < GREEN_LIMIT && reading >= YELLOW_LIMIT)
//     {
//         strip.moderate_flash_yellow();
//         return;
//     }
//     if(reading < YELLOW_LIMIT && reading >= RED_LIMIT)
//     {
//         strip.slow_flash_red();
//         return;
//     }
//     if(reading < RED_LIMIT && reading >= 0)
//     {
//         strip.fast_flash_red();
//         return;
//     }
// }