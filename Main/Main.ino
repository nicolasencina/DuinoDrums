#include <QueueList.h>
#include <SD.h>
#include <TMRpcm.h>
#include "sensor.h"

#define n_sensor 2
#define sound_select_button 2
#define save_button 3
#define clap_pin A0
#define kick_pin A1
#define clap_sound "tom1.wav"
#define kick_sound "ohh.wav"
#define clap_sound_0 "tom1.wav"
#define kick_sound_0 "ohh.wav"
#define clap_sound_1 "tom2.wav"
#define kick_sound_1 "chh.wav"

TMRpcm tmrpcm1;
//TMRpcm tmrpcm2;

const int SSpin = 10;

// Change sounds variables
int buttonState = 0; 
int lastButtonState = 0;
bool sound_change = false;

// Save playing variables
int saveButtonState = 0;
int lastState_SaveButton = 0;
bool saving = false;
int multifacetic_button = 0;
unsigned int iterations = 0;
unsigned int total_iterations = 0;

QueueList <int> states;
QueueList <unsigned int> times;


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
  pinMode(sound_select_button, INPUT);
  
  Serial.begin(9600);       // use the serial port
  tmrpcm1.speakerPin = 11; // salida de audio
  pinMode(12,OUTPUT);
  
  tmrpcm1.speakerPin2 = 5;
  pinMode(2,OUTPUT);

  //tmrpcm2.speakerPin = 10; // salida de audio

  if (!SD.begin(10)) {
    Serial.println("Se ha producido un fallo al iniciar la comunicacion");
    return;
  }
  Serial.println("Se ha iniciado la comunicacion correctamente");

  tmrpcm1.setVolume(1);
  //tmrpcm2.setVolume(1);
}

void change_sounds_layout(bool &sound_change){
  if (sound_change){
    Clap.change_sound(clap_sound_0);
    Kick.change_sound(kick_sound_0);
  }
  else {
    Clap.change_sound(clap_sound_1);
    Kick.change_sound(kick_sound_1);
  }
}

bool DetectRisingEdge(int &lastButton_state, int button_pin){
  // Detect rising pulse of button
  int button_state = digitalRead(button_pin);
  if (button_state != lastButton_state){
    if (button_state == HIGH){
      return true;
    } else{
      return false;
    }
  //return false;
  }
  else {
  return false;
  }
  
}

void PlayRecord(unsigned int total_iterations){
  unsigned int next_time;
  unsigned int actual_iteration = 0;
  int next_state;
  bool found = true;


  for (unsigned int k=0; k<=total_iterations; k++){
    // Extract time and state of the next sound to be played
    if (found){
      next_time = times.pop();
      next_state = states.pop();
      found = false;
    }

    // Wait till k equals the time of the next sound to be played
    if (k == next_time){
      found = true;
      if (next_state == 0){ Clap.play_sound(tmrpcm1, 0); }
      else if (next_state == 1){ Kick.play_sound(tmrpcm1, 1); }
    }
    delay(15);
  }

}


void loop() {

   // Detect rising pulse of button to change sounds of each sensos
  if (DetectRisingEdge(lastButtonState, sound_select_button)){
    sound_change = not sound_change;
    change_sounds_layout(sound_change); 
  }

  if (DetectRisingEdge( lastState_SaveButton, save_button)){
    
    multifacetic_button += 1;

    if (multifacetic_button == 4){ multifacetic_button = 1;}
    
    if (multifacetic_button == 1){
      iterations = 0; // reinit timer
      saving = true;
      Serial.println("Saving Mode");
    }

    else if(multifacetic_button == 2){
      saving = false;
      Serial.println("Saving Mode Finished");
    }

    else if( multifacetic_button == 3){
      Serial.println("Playing Mode");
      PlayRecord(total_iterations);
    }

  }

  // Get the sensors reading
  actualReading[0] = Clap.sensor_read();
  actualReading[1] = Kick.sensor_read();

  who_plays[0] = Clap.should_play(actualReading[0], lastReading[0], ignore_next_hits[0], ignore_counter[0]);
  who_plays[1] = Kick.should_play(actualReading[1], lastReading[1], ignore_next_hits[1], ignore_counter[1]);

  if (who_plays[0]){
    Clap.play_sound(tmrpcm1, 0);
    if (saving){ 
      states.push(0);
      times.push(iterations);
      total_iterations = iterations;
      } 
    }
  if (who_plays[1]){
    Kick.play_sound(tmrpcm1, 1);
    if (saving){ 
      states.push(1);
      times.push(iterations);
      total_iterations = iterations;
      } 
    }
  
  // Actualize last button readings
  lastButtonState = digitalRead(sound_select_button);
  lastState_SaveButton = digitalRead(save_button);


  delay(15); 

  // Store the last sensor readings 
  lastReading[0] = actualReading[0];
  lastReading[1] = actualReading[1];

  
  if (saving) {iterations += 1;} // Increase iterations if saving mode is enabled

}

