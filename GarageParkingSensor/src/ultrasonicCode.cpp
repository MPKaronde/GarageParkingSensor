/*
Class to contain ultrasonic sensor code
*/

#include <Arduino.h>

class Ultrasonic
{
    public:
        // constructor
        Ultrasonic(int echoPin, int trigPin)
        {
            this->echoPin = echoPin;
            this->trigPin = trigPin;

            pinMode(this->trigPin, OUTPUT);
            pinMode(this->echoPin, INPUT);
        }

        // takes a reading and returns distance in mm
        int takeReading()
        {
            unsigned long timing;
            float distance;

            // clear the trigpin
            digitalWrite(this->trigPin, LOW);
            delayMicroseconds(2);

            // send a pulse to trigger the sensor
            digitalWrite(trigPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin, LOW);

            // read the echo pin for sound travel time
            timing = pulseIn(echoPin, HIGH);

            // calculate distance
            distance = (timing * SPEED_OF_SOUND) / 2.0;

            // convert to whole mm and return
            return (int)distance;
        }

    private:
        // sensor pins
        int echoPin;
        int trigPin;

        // speed of sound is 343000mm/s, need mm / microsecond
        const float SPEED_OF_SOUND = .343; 
};