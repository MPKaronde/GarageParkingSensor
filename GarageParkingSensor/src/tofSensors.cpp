#include <Arduino.h>
#include <Adafruit_VL53L0X.h>
#include <Wire.h>

class TOFSensors
{
    public:
        TOFSensors(int xshut_left, int xshut_right):
        XSHUT_LEFT(xshut_left), XSHUT_RIGHT(xshut_right), leftTOF(), rightTOF()
        {
            Wire.begin(21, 22); // SDA = 21, SCL = 22

            pinMode(XSHUT_LEFT, OUTPUT);
            pinMode(XSHUT_RIGHT, OUTPUT);
            // turn off both sensors
            digitalWrite(XSHUT_LEFT, LOW);
            digitalWrite(XSHUT_RIGHT, LOW);
            delay(10);

            // power on sensor 1 and set new address
            digitalWrite(XSHUT_LEFT, HIGH);
            delay(10);
            leftTOF.begin(0x30, &Wire);

            // power on sensor 2 and set new address
            digitalWrite(XSHUT_RIGHT, HIGH);
            delay(10);
            rightTOF.begin(0x31, &Wire);
        }

        // returns measurement from left sensor
        // return -1 if out of range
        int get_left_reading()
        {
            VL53L0X_RangingMeasurementData_t measure;
            leftTOF.rangingTest(&measure, false);
            if(measure.RangeStatus == 4){return -1;}    // out of range
            if(measure.RangeMilliMeter == ALMOST_OUT_OF_RANGE){return -1;} // special case for almost out of range
            return measure.RangeMilliMeter;
        }

        // returns measurement from right sensor
        // return -1 if out of range
        int get_right_reading()
        {
            VL53L0X_RangingMeasurementData_t measure;
            rightTOF.rangingTest(&measure, false);
            if(measure.RangeStatus == 4){return -1;}    // out of range
            if(measure.RangeMilliMeter == ALMOST_OUT_OF_RANGE){return -1;} // special case for almost out of range
            return measure.RangeMilliMeter;
        }

    private:
        // sensor pins
        int XSHUT_LEFT;
        int XSHUT_RIGHT;
        const int ALMOST_OUT_OF_RANGE = 8191; // weird value that seems to be hardcoded into the sensors?

        // sensor objects
        Adafruit_VL53L0X leftTOF;
        Adafruit_VL53L0X rightTOF;

};