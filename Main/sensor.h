

namespace Drums {

	class Sensor{

	private:
		int _input_pin;
		char* _sound;
   
		
	public:
		Sensor(int pin, char* sound){
			_input_pin = pin;
			_sound = sound;
		}

	// Public Functions:
		int sensor_read(){ return analogRead(_input_pin); }
		
		void play_sound(TMRpcm &sound_player){ sound_player.play(_sound);}
		
		void change_sound(char* sound){
			_sound = sound;
		}

		bool should_play(int &sensorReading, int &lastReading, bool &ignore_next_hits, int &ignore_counter){

			int diff = sensorReading - lastReading;

			if (not ignore_next_hits){

				if (sensorReading > 100 && diff > 0){    // IMPORTANTE 100 -> SENSOR THRESHOLD, 0-> DIFF THRESHOLD
					ignore_next_hits = true;
					Serial.print("Playing: ");
					Serial.println(_sound);
					return true; 
					}

				else { return false;}
			}

			else{

				ignore_counter += 1;
				if (ignore_counter > 3){
					ignore_next_hits = false;
			  		ignore_counter = 0;
			  	}
			  	return false;
			}
		}

	};

}
