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

char clap_sound[] = "clap.wav";
const int clap_pin = A0;
Drums::Sensor Clap(clap_pin , clap_sound);

char kick_sound[] = "kick.wav";
const int kick_pin = A1;
Drums::Sensor Kick( kick_pin, kick_sound);

// these constants won't change:
const int SSpin = 10;


int sensorReading = 0;   
int kickReading = 0;

int lastReading = 0;
int counter = 0;

bool ignore_next_hits = false;
int ignore_counter = 0;


int actual_Reading[2];
int last_Reading[2];

void setup() {
  pinMode(SSpin, OUTPUT);
  
  Serial.begin(9600);       // use the serial port
  tmrpcm.speakerPin = 9; // salida de audio

  if (!SD.begin(10)) {
    Serial.println("Se ha producido un fallo al iniciar la comunicacion");
    return;
  }
  Serial.println("Se ha iniciado la comunicacion correctamente");

  tmrpcm.setVolume(1);

}

void loop() {

  sensorReading = Clap.sensor_read();
  kickReading = Kick.sensor_read();

 
  bool should_clap = Clap.should_play(sensorReading, lastReading, ignore_next_hits, ignore_counter);
    
  delay(15);  // delay to avoid overloading the serial port buffer
  lastReading = sensorReading;


}
