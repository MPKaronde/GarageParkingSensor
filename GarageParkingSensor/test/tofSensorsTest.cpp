// #include <Arduino.h>
// #include "tofSensors.cpp"

// TOFSensors *tofSensors;

// void setup() {
//     Serial.begin(115200);
//     delay(1000); // Allow time for serial to initialize

//     tofSensors = new TOFSensors(14, 13); // Example pin numbers for XSHUT_LEFT and XSHUT_RIGHT
// }

// void loop()
// {
//     int leftReading = tofSensors->get_left_reading();
//     int rightReading = tofSensors->get_right_reading();

//     Serial.print("Left TOF Reading: ");
//     if (leftReading == -1) {
//         Serial.println("Out of range");
//     } else {
//         Serial.println(leftReading);
//     }

//     Serial.print("Right TOF Reading: ");
//     if (rightReading == -1) {
//         Serial.println("Out of range");
//     } else {
//         Serial.println(rightReading);
//     }

//     delay(1000); // Delay to avoid tight loop readings
// }