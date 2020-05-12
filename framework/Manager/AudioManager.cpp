/**
 * @file AudioManager.cpp
 * @author Romil Tendulkar
 * @date 10/31/2019
 * @brief This class is used to interact with the Audio System class that implements various FMOD functionalities
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../Source/pch.h"
#include "AudioManager.h"

#define SOUND_PATH "./Resource/Sounds/"


AudioManager::AudioManager()
{
	mAudioSystem = new AudioSystem();
	mAudioSystem->Init();
	mMasterVolume = 0.1f;
}


AudioManager::~AudioManager()
{
	Pause();
	delete mAudioSystem;
}

AudioManager* AudioManager::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new AudioManager();
	} 
	return mInstance;
}

void AudioManager::LoadLoopingAudio(const std::string& path)
{
	std::string soundPath = SOUND_PATH + path;
	mAudioSystem->Loadsound(soundPath, true, true, true);
	mLoopChannel = mAudioSystem->PlaySound(soundPath, Vector3x{ (0), (0), (0) }, mMasterVolume);
}

void AudioManager::LoadAudio(const std::string& path)
{
	std::string soundPath = SOUND_PATH + path;
	if (mChannelMap.find(soundPath) == mChannelMap.end())
	{
		mAudioSystem->Loadsound(soundPath, true, false, true);
		int channel = mAudioSystem->PlaySound(soundPath, Vector3x{ (0), (0), (0) }, mMasterVolume);
		mChannelMap.insert(std::make_pair(soundPath, channel));
	}
	else
	{
		if(!mAudioSystem->IsPlaying(mChannelMap.find(soundPath)->second))
		{
			mAudioSystem->PlaySound(soundPath, Vector3x{ (0), (0), (0) }, mMasterVolume);
			int channel = mAudioSystem->PlaySound(soundPath, Vector3x{ (0), (0), (0) }, mMasterVolume);
			mChannelMap[soundPath] = channel;
		}
	}
	
}

void AudioManager::Resume()
{
	mAudioSystem->ResumeChannel(mLoopChannel);
}

void AudioManager::Pause()
{
	mAudioSystem->PauseChannel(mLoopChannel);
}

void AudioManager::Stop()
{
	mAudioSystem->StopChannel(mLoopChannel);
}

void AudioManager::SetMasterVolume(float volume)
{
	mMasterVolume = volume;
	mAudioSystem->SetMusicvolume(mLoopChannel, mMasterVolume);
	mAudioSystem->SetSFXvolume(mLoopChannel, mMasterVolume);
}

void AudioManager::StopSound(const std::string& path)
{
	//if (mAudioSystem->IsPlaying(mChannelMap[path]))
	{
		mAudioSystem->StopChannel(mChannelMap[path]);
	}
	
}