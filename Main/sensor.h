

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
    
	};

}
