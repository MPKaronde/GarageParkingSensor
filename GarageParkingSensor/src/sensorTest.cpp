// #include <Arduino.h>
// #include <Adafruit_NeoPixel.h>
// #include <Adafruit_VL53L0X.h>
// #include <Wire.h>

// #define XSHUT_LEFT 14
// #define XSHUT_RIGHT 13


// // Create Distance Sensor object
// Adafruit_VL53L0X leftSensor = Adafruit_VL53L0X();
// Adafruit_VL53L0X rightSensor = Adafruit_VL53L0X();


// void setup()
// {
//     Serial.begin(115200);
//     Wire.begin(21, 22); // SDA = 21, SCL = 22

//     pinMode(XSHUT_LEFT, OUTPUT);
//     pinMode(XSHUT_RIGHT, OUTPUT);

//     // turn off both sensors
//     digitalWrite(XSHUT_LEFT, LOW);
//     digitalWrite(XSHUT_RIGHT, LOW);
//     delay(10);

//     // power on sensor 1 and set new address
//     digitalWrite(XSHUT_LEFT, HIGH);
//     delay(10);
//     if (!leftSensor.begin(0x30, &Wire)) {
//         Serial.println("Failed to initialize sensor 1");
//         while (1);
//     }

//     // power on sensor 2 and set new address
//     digitalWrite(XSHUT_RIGHT, HIGH);
//     delay(10);
//     if (!rightSensor.begin(0x31, &Wire)) {
//         Serial.println("Failed to initialize sensor 2");
//         while (1);
//     }
// }

// void loop() {
//     VL53L0X_RangingMeasurementData_t measure1;
//     VL53L0X_RangingMeasurementData_t measure2;

//     leftSensor.rangingTest(&measure1, false);
//     rightSensor.rangingTest(&measure2, false);

//     Serial.print("Sensor 1: ");
//     if (measure1.RangeStatus != 4)
//         Serial.print(measure1.RangeMilliMeter);
//     else
//         Serial.print("Out of range");
    
//     Serial.print(" mm\tSensor 2: ");
//     if (measure2.RangeStatus != 4)
//         Serial.print(measure2.RangeMilliMeter);
//     else
//         Serial.print("Out of range");
    
//     Serial.println(" mm");

//     delay(500);
// }
