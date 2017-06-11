//#ifndef sensor.h
//# define sensor.h

namespace Drums {

	class Sensor{

	private:
		int _input_pin;
		String _sound;
        
	public:
		Sensor(int pin, String sound){
			_input_pin = pin;
			_sound = sound;
		}
   
		int sensor_read(){ return analogRead(_input_pin); }
	  void play_sound(TMRpcm &sound_player){ sound_player.play("clap.wav");}
	
	};

}
