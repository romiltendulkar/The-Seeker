/**
 * @file AudioManager.h
 * @author Romil Tendulkar
 * @date 10/31/2019
 * @brief This class is used to interact with the Audio System class that implements various FMOD functionalities
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */


#pragma once


#include "../Source/pch.h"
#include "../Source/Audio/AudioSystem.h"

class AudioManager
{
public:
	/**
	Creates and initializes an object of the AudioSystem class.
	It also initializes the volume.
	*/
	AudioManager();

	/**
	Stops all Playing Audio and then destroys the AudioSystem Object
	*/
	~AudioManager();

	/**
	This function creates and returns an instance of Audio Manager.
	If an instance already exists it is returned instead.

	\param path | the name of the audio file stored in Resources\Sounds
	*/
	void LoadLoopingAudio(const std::string& path);

	/**
	The loadLoopingAudio function is used to load in and play background audio or other looped audio

	\param path | the name of the audio file stored in Resources\Sounds
	*/
	void LoadAudio(const std::string& path);

	/**
	The loadAudio function is used to load in and play audio that needs to be played once or as an event

	\param path | the name of the audio file stored in Resources\Sounds
	*/
	void Stop();

	/**
	Resumes all paused audio
	*/
	void Resume();

	/**
	Pauses all playing audio
	*/
	void Pause();

	/**
	This function is used to set Master volume to a desired level

	\param Volume | The volume level you want to set the master volume at
	*/
	void SetMasterVolume(float volume);

	/**
	Stops All Playing Audio
	*/
	void StopSound(const std::string& path);
	
	/**
	This function return Manager itself

	\return AudioManager* | The manager its pointer
	*/
	static AudioManager* GetInstance();

	AudioSystem* mAudioSystem;
private:
	static AudioManager* mInstance;
	float mMasterVolume;
	int mLoopChannel;
	std::map<std::string, int> mChannelMap;
};
