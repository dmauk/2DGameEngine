#pragma once

#include <SDL\SDL_mixer.h>
#include <string>
#include <map>

using namespace std;

namespace GameEngine2D {

	class SoundEffect {
	public:
		friend class AudioEngine;

		///Plays the audio file
		///@param loops: If loops == -1, loop forever, otherwise loop for loops + 1 times
		void play(int loops = 0);

		//pause

		//repeat
	private:

		Mix_Chunk* m_chunk = nullptr;
	};

	class Music {
	public:
		friend class AudioEngine;

		///Plays the audio file
		///@param loops: If loops == -1, loop forever, otherwise loop for specified times (1 for 1 time etc
		void play(int loops = -1);

		static void pause(); //Applies to the only music playing, essentially global
		static void stop();
		static void resume();

	private:
		Mix_Music* m_music = nullptr;
	};

	class AudioEngine
	{
	public:
		AudioEngine();
		~AudioEngine();

		void init();
		void destroy();

		//Since soundeffect is a rapper of a pointer, it's only 8 bytes, so returning a copy of it is no big deal... for now ayyyy
		SoundEffect loadSoundEffect(const string& filePath); //Sound effects are cached
		Music loadMusic(const string& filePath);

	private:
		bool m_isInitialized = false;

		std::map<string, Mix_Chunk*> m_effectMap;
		std::map<string, Mix_Music*> m_musicMap;
	};

}

