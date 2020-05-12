/**
 * @file AudioSystem.h
 * @author Romil Tendulkar
 * @date 10/30/2019
 * @brief This class implements all the FMOD functionality required in the engine
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#pragma once
#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H
#include "../pch.h"
#include <fmod_studio.hpp>
#include <fmod.hpp>
#include "../Util/Vector3D.h"

struct Implementation {

	/**
	This function initializes the FMOD System
	*/
	Implementation();

	/**
		This function destroys the FMOD system and all data pertaining to it
	*/
	~Implementation();

	/**
		This function calls the update on the FMOD System
	*/
	void Update();

	FMOD::Studio::System* mpStudioSystem;
	FMOD::System* mpSystem;

	int mnNextChannelId;

	typedef std::map<std::string, FMOD::Sound*> mSoundMap;
	typedef std::map<int, FMOD::Channel*> mChannelMap;
	typedef std::map<std::string, FMOD::Studio::EventInstance*> mEventMap;
	typedef std::map<std::string, FMOD::Studio::Bank*> mBankMap;

	mBankMap mBanks;
	mEventMap mEvents;
	mSoundMap mSounds;
	mChannelMap mChannels;
};


class AudioSystem
{
public:
	AudioSystem();

	/**
		Calls the Shutdown function to stop the FMOD System
	*/
	~AudioSystem();

	/**
		Initializes the data structure storing the FMOD System
	*/
	static void Init();

	/**
		Updates the data structure storing the FMOD System
	*/
	static void Update();
	
	/**
		deletes the data structure that has the FMOD System
	*/
	static void Shutdown();
	
	/**
		this function performs FMOD error checks
	*/
	static int ErrorCheck(FMOD_RESULT result);

	/**
		Load Sound banks in FMOD

		\param strBankName | name of the sound bank
		\param flags | parameters to set for the sound bank
	*/
	void LoadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
	
	/**
		Loads sounds as events into FMOD

		\param strEventName | the path and name of the sound to be loaded as an event
	*/
	void LoadEvent(const std::string& strEventName);
	
	/**
	This function is used to load sound into the FMOD system along with the parameters pertaining to it

	\param strSoundName | path and name of the sound file to be loaded
	\param b3d | boolean value for whether the sound is in 3D
	\param bLooping | boolean value for whether the sound is looping
	\param bStream | boolean value for whether the sound is to be streamed
	*/
	void Loadsound(const std::string& strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false);
	
	/**
		This function is used to unload sound from FMOD

		\param strSoundName | path and name of the file to be unloaded
	*/
	void UnLoadSound(const std::string& strSoundName);
	
	/**
		Function to tell the FMOD system of play the sound.
		Gives the sound a channel to play or returns the channel if it was already played

		\param strSoundName | path and name of the sound to be played
		\param vPosition | position of the sound in 3d Space
		\param fVolumedB | volume the sound is to be played at
	*/
	int PlaySound(const std::string& strSoundName, const Vector3x& vPos = Vector3x{ 0, 0, 0 }, float fVolumedB = 0.0f);
	
	/**
		Plays sounds as events into FMOD

		\param strEventName | the path and name of the sound to be Played as an event
	*/
	void PlayEvent(const std::string& strEventName);
	
	/**
		Stops the all Sounds playing in the channel

		\param ChannelId | The id of the channel to be stopped
	*/
	void StopChannel(int nChannelId);

	/**
		Stops The event if its being played

		\param strEventName | the path and name of the event to be stopped
		\param bImmediate | boolean for whether the Event is to be stopped immediately
	*/
	void StopEvent(const std::string& strEventName, bool bImmediate = false);
	
	/**
	Get a value of a specific parameter for the event

	\param strEventName | path and name of the event
	\param strParameterName | The name of the parameter that you want the value for
	\param parameter | the value in float of the parameter
*/
	void GeteventParameter(const std::string& strEventName, const std::string& strEventParameter, float* parameter);
	
	/**
		Set a value of a specific parameter for the event

		\param strEventName | path and name of the event
		\param strParameterName | The name of the parameter that you want to set the value for
		\param parameter | the value in float of the parameter
	*/
	void SetEventParameter(const std::string& strEventName, const std::string& strParameterName, float fValue);
	void StopAllChannels();
	
	/**
		Set the position in 3D space for the channel

		\param nChannelId | The id of the channel for which the position is to be set
		\param vPosition | The 3D Space position of the sound
	*/
	void SetChanneledPosition(int nChannelId, const Vector3x& vPosition);
	
	/**
		Set the volume for the background music

		\param nChannelId | The id of the channel for background music
		\param fVolumedB | The volume to set for background music
	*/
	void SetMusicvolume(int nChannelId, float fVolumedB);
	
	/**
		Set the volume for the sound effects

		\param nChannelId | The id of the channel for background music
		\param fVolumedB | The volume to set for sound effects
	*/
	void SetSFXvolume(int nChannelId, float fVolumedB);
	
	/**
		Pauses all Sounds playing in the channel

		\param ChannelId | The id of the channel to be Paused
	*/
	void PauseChannel(int ChannelID);

	/**
	Resumes all Sounds playing in the channel

	\param ChannelId | The id of the channel to be Resumed
*/
	void ResumeChannel(int ChannelID);

	/**
		This function returns the next available FMOD Channel

		\return returns next available FMOD Channel
	*/
	int SendChannelId();

	/**
		This function checks if a channel is playing sound

		\return returns whether a sound is playing on the channel or not
	*/
	bool IsPlaying(int nChannelId) const;
	
	/**
		Checks if an event is being played

		\param strEventName | the path and name of the event
	*/
	bool IsEventPlaying(const std::string& strEventName) const;
	
	/**
		This function converts from a dB value to a float value
	*/
	float dbToVolume(float db);
	
	/**
		This function converts from a float value to a dB value
	*/
	float VolumeTodb(float volume);
	
	/**
		This function changes the engines vector data to FMODs vectors

		\param vPosition | the vector to be converted
	*/
	FMOD_VECTOR VectorToFmod(const Vector3x& vPosition);

};



#endif // !AUDIOSYSTEM_H

