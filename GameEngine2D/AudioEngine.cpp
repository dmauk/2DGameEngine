#include "AudioEngine.h"
#include "ErrorHandling.h"

namespace GameEngine2D {

	void SoundEffect::play(int loops /*default = 0*/)
	{
		if (Mix_PlayChannel(-1, m_chunk, loops) == -1) //Mix_playChannel returns the channel it will play on 
		{
			if (Mix_PlayChannel(0, m_chunk, loops) == -1) //Causes audio choking, implement better method -> use oldest channel first
			{
				fatalError("Mix_PlayChannel error: " + string(Mix_GetError()));
			}
		}
	}

	void Music::play(int loops/*default = -1*/)
	{
		if (Mix_PlayMusic(m_music, loops) == -1) //Mix_playChannel returns the channel it will play on 
		{
			fatalError("Mix_PlayMusic error: " + string(Mix_GetError()));
		}
	}

	void Music::pause()
	{
		Mix_PauseMusic();
	}

	void Music::resume()
	{
		Mix_ResumeMusic();
	}

	void Music::stop()
	{
		Mix_HaltMusic();
	}

	AudioEngine::AudioEngine()
	{
	}


	AudioEngine::~AudioEngine()
	{
		destroy();
	}

	void AudioEngine::init() //Make sure audio initialization occurs after SDL has been initialized
	{

		if (m_isInitialized)
		{
			fatalError("Tried to initialize AudioEngine twice");
		}

		//Can be a bitwise combination of MIX_INIT_FAC, MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1)
		{
			fatalError("Mix_Init error: " + string(Mix_GetError()));
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		{
			fatalError("Mix_OpenAudio error: " + string(Mix_GetError()));
		}
		
		m_isInitialized = true;
	}

	void AudioEngine::destroy()
	{
		if (m_isInitialized)
		{
			m_isInitialized = false;

			for (auto it = m_effectMap.begin(); it!= m_effectMap.end(); it++)
			{
				Mix_FreeChunk(it->second);
			}

			//for each method for music as an example
			for (auto& it : m_musicMap)
			{
				Mix_FreeMusic(it.second);
			}

			m_effectMap.clear();
			m_musicMap.clear();

			Mix_CloseAudio();
			Mix_Quit();
		}
	}

	SoundEffect AudioEngine::loadSoundEffect(const string& filePath)
	{
		//Try to find audio in the cache
		auto it = m_effectMap.find(filePath);

		SoundEffect effect;

		if (it == m_effectMap.end()) //If it is the end of the map, it failed to find the file in the cache
		{
			//Failed to find it, must load
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
			if (chunk == nullptr)
			{
				fatalError("Mix_LoadWAV error: " + string(Mix_GetError()));
			}

			effect.m_chunk = chunk;
			m_effectMap[filePath] = chunk; //Add node if it doesn't already exist
		}
		else
		{
			//It's already cached
			effect.m_chunk = it->second;
		}

		return effect;
	}

	Music AudioEngine::loadMusic(const string& filePath)
	{
		//Try to find audio in the cache
		auto it = m_musicMap.find(filePath);

		Music music;

		if (it == m_musicMap.end()) //If it is the end of the map, it failed to find the file in the cache
		{
			//Failed to find it, must load
			Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
			if (mixMusic == nullptr)
			{
				fatalError("Mix_LoadMUS error: " + string(Mix_GetError()));
			}

			music.m_music = mixMusic;
			m_musicMap[filePath] = mixMusic; //Add node if it doesn't already exist
		}
		else
		{
			//It's already cached
			music.m_music = it->second;
		}

		return music;
	}

}

