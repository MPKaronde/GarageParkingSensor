#include <Arduino.h>
#include <Adafruit_VL53L0X.h>
#include "ultrasonic.cpp"
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
            // set up sensor arrays
            int readings[3] = {0};                  // array of readings
            readings[0] = tof.get_left_reading();
            readings[1] = tof.get_right_reading();
            readings[2] = ultrasonic.takeReading();
            bool exclude[3] = {false};              // whether to use reading at given index

            // handle errors
            int numErrors = 0;
            for(int i = 0; i < 3; i++)
            {
                if(readings[i] < -1)
                {
                    exclude[i] = true;
                    numErrors++;
                }
            }
            // not enough readings to trust
            if (numErrors > 1)
            { 
                return -2;
            }

            // handle out of bounds
            int numOutOfBounds = 0;
            for(int i = 0; i < 3; i++)
            {
                if(readings[i] == -1 && exclude[i] == false)
                {
                    exclude[i] = true;
                    numOutOfBounds++;
                }
            }
            // def out of range
            if (numOutOfBounds == 3)
            {
                return -1;
            }
            // maybe out of range
            if(numOutOfBounds == 2)
            {
                // find the val that was in range
                int realNum = 0;
                for(int i = 0; i < 3; i++)
                {
                    if(!exclude[i] && readings[i] >= 0)
                    {
                        realNum = readings[i];
                    }
                }

                // realNum is reasonable, return it
                if(realNum > HIGH_DISTANCE)
                {
                    return realNum;
                }
                // realNum is not reasonable, return -1
                else
                {
                    return -1;
                }
            }

            // find avg val
            int numReadingsUsed = 0;
            double avgReading = 0;
            for(int i = 0; i < 3; i++)
            {
                if(!exclude[i])
                {
                    avgReading += readings[i];
                    numReadingsUsed++;
                }
            }
            avgReading = avgReading / (double)numReadingsUsed;

            // if only 2 vals, make sure they are within good range
            if (numReadingsUsed == 2)
            {
                int valA = 0;
                int valB = 0;
                for(int i = 0; i < 3; i++)
                {
                    if(!exclude[i])
                    {
                        if(valA == 0)
                        {
                            valA = readings[i];
                        }
                        else
                        {
                            valB = readings[i];
                        }
                    }
                }

                int diff = valA - valB;
                if(diff < 0)
                {
                    diff = -diff;
                }
                if(diff < LARGE_GAP)
                {
                    return avgReading;
                }
                return -2;  // vals are too far apart, reading cant be trusted
            }

            // all 3 vals must have been used, determine if any are out of range
            int numOff = 0;
            for (int i = 0; i < 3; i++)
            {
                // reset exclude for use to determine if any sensor seems off
                exclude[i] = false;
                int diff = avgReading - readings[i];
                if(diff < 0)
                {
                    diff = -diff;
                }
                if(diff > LARGE_GAP)
                {
                    exclude[i] = true;
                    numOff++;
                }
            }

            // valid avg calc
            if (numOff == 0)
            {
                return avgReading;
            }

            // readings cant be trusted
            if(numOff > 1)
            {
                return -2;
            }

            // exlcude the bad reading and recalculate
            avgReading = 0.0;
            for(int i = 0; i < 3; i++)
            {
                if(!exclude[i])
                {
                    avgReading += readings[i];
                }
            }
            avgReading /= 2.0;
            
            // return final avg reading
            return (int)avgReading;
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
        int decided_history[10] = {0};  // last 10 readings from the consensus of all sensors

        // arbitrary constants for calculations
        const int HIGH_DISTANCE = 600;  // anything beyond this is considered "far"
        const int LARGE_GAP = 150;      // anything considered too far apart to be similar readings, in mm


        // total the number of sensors with erroneous readings
        int total_bools(bool a, bool b, bool c)
        {
            int ret = 0;
            if(!a)
            {
                ret++;
            }
            if(!b)
            {
                ret++;
            }
            if(!c)
            {
                ret++;
            }
            return ret;
        }


};