#include <SD.h>
#include <TMRpcm.h>
#include "sensor.h"

#define n_sensor 2
#define clap_pin A0
#define kick_pin A1
#define clap_sound "tom1.wav"
#define kick_sound "ohh.wav"

TMRpcm tmrpcm1;
//TMRpcm tmrpcm2;

const int SSpin = 6;

//////////// Sensor Objects /////////////
// Clap
Drums::Sensor Clap(clap_pin , clap_sound);
// Kick
Drums::Sensor Kick(kick_pin, kick_sound);

// States
int actualReading[n_sensor];
int lastReading[n_sensor] = {0, 0};
bool ignore_next_hits[n_sensor] = {false, false};
int ignore_counter[n_sensor] = {0, 0};
bool who_plays[n_sensor] = {false};


void setup() {
  pinMode(SSpin, OUTPUT);
  
  Serial.begin(9600);       // use the serial port
  tmrpcm1.speakerPin = 9; // salida de audio

  //tmrpcm2.speakerPin = 10; // salida de audio

  if (!SD.begin(10)) {
    Serial.println("Se ha producido un fallo al iniciar la comunicacion");
    return;
  }
  Serial.println("Se ha iniciado la comunicacion correctamente");

  tmrpcm1.setVolume(1);
  //tmrpcm2.setVolume(1);
}

void loop() {

  actualReading[0] = Clap.sensor_read();
  actualReading[1] = Kick.sensor_read();

  who_plays[0] = Clap.should_play(actualReading[0], lastReading[0], ignore_next_hits[0], ignore_counter[0]);
  who_plays[1] = Kick.should_play(actualReading[1], lastReading[1], ignore_next_hits[1], ignore_counter[1]);

  if (who_plays[0]){
    Clap.play_sound(tmrpcm1);
    }
  if (who_plays[1]){
    Kick.play_sound(tmrpcm1);
    }  

  delay(15);  
  lastReading[0] = actualReading[0];
  lastReading[1] = actualReading[1];

}
