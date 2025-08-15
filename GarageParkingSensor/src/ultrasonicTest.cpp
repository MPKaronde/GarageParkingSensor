// #include <Arduino.h>
// #include "ultrasonic.cpp"

// const int TRIGPIN = 2;
// const int ECHOPIN = 34;

// Ultrasonic ultrasonic(ECHOPIN, TRIGPIN);

// void setup() {
//     Serial.begin(115200);
//     delay(1000); // Allow time for serial to initialize
// }

// void loop() {
//     int distance = ultrasonic.takeReading();
//     Serial.print("Distance: ");
//     if(distance == -1)
//     {
//         Serial.println("Out of range");
//     }
//     else 
//     {
//         Serial.println(distance);
//     }

//     ultrasonic.readDelay(); // Delay to avoid tight loop readings
// }


