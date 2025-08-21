#include <Arduino.h>
#include <Adafruit_VL53L0X.h>
#include "ultrasonic.cpp"
#include "tofSensors.cpp"

class Sensors {
    public:
        // initialize sensors and constants, most actual setup code is handled w/in sensor constructors
        Sensors(int xshut_left, int xshut_right, int trig, int echo):  
        XSHUT_LEFT(xshut_left), XSHUT_RIGHT(xshut_right), TRIG(trig), ECHO(echo),
        ultrasonic(echo, trig), tof(XSHUT_LEFT, XSHUT_RIGHT)
        {
            left_tof_history = new int[HISTORY_SIZE];
            right_tof_history = new int[HISTORY_SIZE];
            ultrasonic_history = new int[HISTORY_SIZE];
            decided_history = new int[HISTORY_SIZE];
        }

        // take a reading from all three sensors and find a consensus to return
        // returns a distance in mm, -1 if out of range, -2 if readings cant be trusted, -3 if sensors are verified not working
        int get_reading()
        {
            // set up sensor arrays
            int readings[3] = {0};                                                          // array of readings
            readings[0] = tof.get_left_reading();
            readings[1] = tof.get_right_reading();
            readings[2] = ultrasonic.takeReading();
            bool exclude[3] = {false};                                                      // whether to use reading at given index

            // handle errors
            int numErrors = count_readings(readings, ERROR_READING, exclude, true);
            if (numErrors > 1){ return handle_ret(readings, ERROR_READING);}                // not enough readings to trust

            // handle out of bounds
            int numOutOfBounds = count_readings(readings, OUT_OF_RANGE, exclude, true);
            if (numOutOfBounds == 3){ return handle_ret(readings, OUT_OF_RANGE);}           // reading is def out of range
            if (numOutOfBounds == 2)                                                        // maybe out of range
            {
                int realNum = get_single_value(readings, exclude);                          // find the val that was in range
                if(realNum > HIGH_DISTANCE){ return handle_ret(readings, realNum); }        // realNum is reasonable, return it
                else{ return handle_ret(readings, OUT_OF_RANGE); }                          // realNum is not reasonable, return -1
            }

            // find the average of the readings
            int avgReading = get_average(readings, exclude);
            if (total_exclude(exclude) == 2)                                                // if only 2 vals, make sure they are within good range
            {
                return handle_ret(readings, handle_two_val_avg(readings, exclude, avgReading));
            }

            // all 3 vals must have been used, determine if any are out of range
            int numOff = 0;
            int median = get_median(readings);                                               // find the median of the readings
            for (int i = 0; i < 3; i++)
            {
                exclude[i] = false;                                                         // reset exclude for use to determine if any sensor seems off
                int diff = absolute_difference(median, readings[i]);                        // use median rather than avg to prevent outliers from skewing the results
                if(diff > LARGE_GAP)                                                        // if reading is too far from average, mark it as excluded    
                {
                    exclude[i] = true;
                    numOff++;
                }
            }

            if (numOff == 0){ return handle_ret(readings, avgReading);}                     // valid avg calc
            if(numOff > 1){ return handle_ret(readings, ERROR_READING);}                    // readings cant be trusted
            avgReading = get_average(readings, exclude);                                    // exclude the bad reading and recalculate
            return handle_ret(readings, avgReading);                                        // return final avg reading
        }

        // check recent history for all sensors and determine if any seems to not be working
        // set given sensor working bool to false if determined to fail
        // return 0 if everything ok, 1 if a single sensor is unreliable, 2 if two or more sensors are unreliable
        int verify_sensors()
        {
            return 0;                                                                        // assume all sensors are working for now
        }

        // for when other code members need this constant
        int get_high_distance()
        {
            return HIGH_DISTANCE;
        }


        /*
        series of functions to get readings from individual sensors for debugging
        */
        int get_left_reading()
        {
            return tof.get_left_reading();
        }

        int get_right_reading()
        {
            return tof.get_right_reading();
        }

        int get_ultrasonic_reading()
        {
            return ultrasonic.takeReading();
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
        const int HISTORY_SIZE = 10; // size of the history arrays
        int* left_tof_history;
        int* right_tof_history;
        int* ultrasonic_history;
        int* decided_history;   // history of decided readings
        int history_index = 0;  // index to track where to write next in the history arrays

        // arbitrary constants for calculations
        const int HIGH_DISTANCE = 600;  // anything beyond this is considered "far"
        const int LARGE_GAP = 150;      // anything considered too far apart to be similar readings, in mm

        // signal constants
        const int ERROR_READING = -2;  // reading that indicates sensor error
        const int OUT_OF_RANGE = -1;   // reading that indicates sensor is out of range


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

        // returns the absolute difference between two integers
        int absolute_difference(int a, int b)
        {
            int diff = a - b;
            if(diff < 0)
            {
                diff = -diff;
            }
            return diff;
        }

        // returns the average of the given values, excluding any that are marked as excluded
        // returns OUT_OF_RANGE if no valid readings
        int get_average(int vals[], bool exclude[])
        {
            int numReadingsUsed = 0;
            double avgReading = 0;
            for(int i = 0; i < 3; i++)
            {
                if(!exclude[i])
                {
                    avgReading += vals[i];
                    numReadingsUsed++;
                }
            }
            if(numReadingsUsed == 0)
            {
                return OUT_OF_RANGE; // no valid readings
            }
            return (int)(avgReading / (double)numReadingsUsed);
        }

        // returns the total number of sensors that are excluded
        int total_exclude(bool exclude[])
        {
            int total = 0;
            for(int i = 0; i < 3; i++)
            {
                if(exclude[i])
                {
                    total++;
                }
            }
            return total;
        }

        // return num values in readings with given value
        // if set_exclude is true, also mark those readings as excluded in the exclude array
        int count_readings(int readings[], int value, bool* exclude, bool set_exclude)
        {
            int count = 0;
            for(int i = 0; i < 3; i++)
            {
                if(readings[i] == value)
                {
                    count++;
                    if(set_exclude && exclude != nullptr)
                    {
                        exclude[i] = true;  // mark this reading as excluded
                    }
                }
            }
            return count;
        }

        // for use when only one val is not excluded
        // returns the value that is not excluded
        int get_single_value(int readings[], bool exclude[])
        {
            for(int i = 0; i < 3; i++)
            {
                if(!exclude[i])
                {
                    return readings[i];
                }
            }
            return OUT_OF_RANGE; // should never happen
        }

        // for when only two vals are used in the average calculation
        // returns the average of the two vals if they are within a reasonable range, otherwise returns ERROR_READING
        // assumes readings is an array of 3 values, exclude is an array of 3 bools indicating which vals to use
        // avgReading is the average of the two vals, used to determine if they are close enough
        // returns ERROR_READING if the two vals are too far apart, otherwise returns avgReading
        int handle_two_val_avg(int readings[], bool exclude[], int avgReading)
        {
                int valA = OUT_OF_RANGE;
                int valB = 0;
                for(int i = 0; i < 3; i++)
                {
                    if(!exclude[i])
                    {
                        if(valA == OUT_OF_RANGE)
                        {
                            valA = readings[i];
                        }
                        else
                        {
                            valB = readings[i];
                        }
                    }
                }

                int diff = absolute_difference(valA, valB);
                if(diff < LARGE_GAP)
                {
                    return avgReading;
                }
                return ERROR_READING;  // vals are too far apart, reading cant be trusted
        }

        // updates the history arrays
        // calls verify sensors if appropriate, return -3 if sensors are not working
        // otherwise returns whatever value is passed in
        int handle_ret(int readings[], int ret)
        {
            // update history arrays
            left_tof_history[history_index] = readings[0];
            right_tof_history[history_index] = readings[1];
            ultrasonic_history[history_index] = readings[2];
            decided_history[history_index] = ret;
            history_index++;

            // time to verify sensors 
            int ver = 0;
            if(history_index >= HISTORY_SIZE)
            {
                history_index = 0;  // reset index to start of arrays
                ver = verify_sensors();  // check if sensors are still working
            }
            if(ver != 0)
            {
                return -3;  // sensors are not working, return error code
            }

            return ret;  // return whatever value was passed in
        }

        // returns the median value of an array of 3 ints
        int get_median(int vals[])
        {
            if(vals[0] > vals[1])
            {
                if(vals[0] < vals[2])
                {
                    return vals[0];  // vals[0] is the median
                }
                else if(vals[1] > vals[2])
                {
                    return vals[1];  // vals[1] is the median
                }
                else
                {
                    return vals[2];  // vals[2] is the median
                }
            }
            else
            {
                if(vals[0] > vals[2])
                {
                    return vals[0];  // vals[0] is the median
                }
                else if(vals[1] < vals[2])
                {
                    return vals[1];  // vals[1] is the median
                }
                else
                {
                    return vals[2];  // vals[2] is the median
                }
            }
        }
};