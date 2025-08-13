#include <Arduino.h>
#include <Adafruit_VL53L0X.h>
#include "ultrasonicCode.cpp"
#include "tofSensors.cpp"

class Sensors {
    public:
        // initialize sensors and constants, most actual setup code is handled w/in sensor constructors
        Sensors(int xshut_left, int xshut_right, int trig, int echo):  
        XSHUT_LEFT(xshut_left), XSHUT_RIGHT(xshut_right), TRIG(trig), ECHO(echo),
        ultrasonic(echo, trig), tof(XSHUT_LEFT, XSHUT_RIGHT){}

        // take a reading from all three sensors and find a consensus to return
        // returns a distance in mm, -1 if out of range, -2 if readings cant be trusted
        int get_reading()
        {
        }

        // check recent history for all sensors and determine if any seems to not be working
        // set given sensor working bool to false if determined to fail
        // return 0 if everything ok, 1 if a single sensor is unreliable, 2 if two or more sensors are unreliable
        int verify_sensors()
        {
        }

    private:
        // TOF sensor pins
        int XSHUT_LEFT;
        int XSHUT_RIGHT;

        // Ultrasonic sensor pins
        int TRIG;
        int ECHO;

        // Sensor objects
        Ultrasonic ultrasonic;
        TOFSensors tof;

        // whether each given sensor is working
        bool left_tof_working = true;
        bool right_tof_working = true;
        bool ultrasonic_working = true;

        // last few readings from each sensor to diagnose issues
        int left_tof_history[10] = {0};
        int right_tof_history[10] = {0};
        int ultrasonic_history[10] = {0};
};