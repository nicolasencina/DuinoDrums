/* Knock Sensor

   This sketch reads a piezo element to detect a knocking sound.
   It reads an analog pin and compares the result to a set threshold.
   If the result is greater than the threshold, it writes
   "knock" to the serial port, and toggles the LED on pin 13.

   The circuit:
    * + connection of the piezo attached to analog in 0
    * - connection of the piezo attached to ground
    * 1-megohm resistor attached from analog in 0 to ground

   http://www.arduino.cc/en/Tutorial/Knock

   created 25 Mar 2007
   by David Cuartielles <http://www.0j0.org>
   modified 30 Aug 2011
   by Tom Igoe

   This example code is in the public domain.

 */

#include <SD.h>
#include <TMRpcm.h>
#include "sensor.h"

File Archivo;
File Music;

TMRpcm tmrpcm;

String clap_sound = "clap.wav";
const int clap_pin = A0;
Drums::Sensor Clap(clap_pin , clap_sound);

// these constants won't change:
const int ledPin = 13;      // led connected to digital pin 13
const int SSpin = 10;
//const int knockSensor = A0; // the piezo is connected to analog pin 0
const int threshold = 100;  // threshold value to decide when the detected sound is a knock or not


// these variables will change:
int sensorReading = 0;      // variable to store the value read from the sensor pin
int lastReading = 0;
int counter = 0;
int diff = 0;
int diff_treshold = 0;
bool ignore_next_hits = false;
int ignore_counter = 0;
bool sound_1 = true;

void setup() {
  pinMode(ledPin, OUTPUT); // declare the ledPin as as OUTPUT
  pinMode(SSpin, OUTPUT);
  
  Serial.begin(9600);       // use the serial port
  tmrpcm.speakerPin = 9; // salida de audio

  if (!SD.begin(10)) {
    Serial.println("Se ha producido un fallo al iniciar la comunicacion");
    return;
  }
  Serial.println("Se ha iniciado la comunicacion correctamente");

  tmrpcm.setVolume(1);
  //tmrpcm.play("clap-808.wav");
  
}

void loop() {
  // read the sensor and store it in the variable sensorReading:
  //sensorReading = analogRead(knockSensor);
  sensorReading = Clap.sensor_read();
  Serial.println(sensorReading);

  diff = sensorReading - lastReading;

  if (not ignore_next_hits){
  
    if (sensorReading > threshold && diff > diff_treshold){
      ignore_next_hits = true;
      counter += 1;
    
      Serial.print("Counter: ");
      Serial.println(counter);

      if (sound_1){
        //Serial.println("Playing first sound");
        tmrpcm.quality(1);
        tmrpcm.play("clap.wav");
      
        sound_1 = not sound_1;
        }
      else {
        //Serial.println("Both sounds");
        tmrpcm.play("clap.wav");
        sound_1 = not sound_1;
        }
      
    }
  }
  
  else{
    ignore_counter += 1;
    if (ignore_counter > 3){
      ignore_next_hits = false;
      ignore_counter = 0;
      }
    }    
    
  delay(15);  // delay to avoid overloading the serial port buffer
  lastReading = sensorReading;


}
