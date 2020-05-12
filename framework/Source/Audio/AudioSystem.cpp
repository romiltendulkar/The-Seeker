/**
 * @file AudioSystem.cpp
 * @author Romil Tendulkar
 * @date 10/30/2019
 * @brief This class implements all the FMOD functionality required in the engine
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../pch.h"
#include "AudioSystem.h"


Implementation::Implementation() 
{
	mpStudioSystem = NULL;
	AudioSystem::ErrorCheck(FMOD::Studio::System::create(&mpStudioSystem));
	AudioSystem::ErrorCheck(mpStudioSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));

	mpSystem = NULL;
	AudioSystem::ErrorCheck(mpStudioSystem->getLowLevelSystem(&mpSystem));
}


Implementation::~Implementation() 
{
	AudioSystem::ErrorCheck(mpStudioSystem->unloadAll());
	AudioSystem::ErrorCheck(mpStudioSystem->release());
	mChannels.clear();
	mSounds.clear();
}


void Implementation::Update() 
{
	std::vector<mChannelMap::iterator> pStoppedChannels;
	for (auto it = mChannels.begin(), itEnd = mChannels.end(); it != itEnd; ++it)
	{
		bool bIsPlaying = false;
		it->second->isPlaying(&bIsPlaying);
		if (!bIsPlaying)
		{
			pStoppedChannels.push_back(it);
		}
	}
	for (auto& it : pStoppedChannels)
	{
		mChannels.erase(it);
	}
	AudioSystem::ErrorCheck(mpStudioSystem->update());
}

Implementation* sgpImplementation = nullptr;

AudioSystem::AudioSystem()
{
}


AudioSystem::~AudioSystem()
{
	Shutdown();
}


void AudioSystem::Init() 
{
	sgpImplementation = new Implementation;
}


void AudioSystem::Update() 
{
	sgpImplementation->Update();
}


void AudioSystem::Loadsound(const std::string& strSoundName, bool b3d, bool bLooping, bool bStream)
{
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt != sgpImplementation->mSounds.end())
		return;

	FMOD_MODE eMode = FMOD_DEFAULT;
	eMode |= b3d ? FMOD_3D : FMOD_2D;
	eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

	FMOD::Sound* pSound = nullptr;
	AudioSystem::ErrorCheck(sgpImplementation->mpSystem->createSound(strSoundName.c_str(), eMode, nullptr, &pSound));
	if (pSound) 
	{
		sgpImplementation->mSounds[strSoundName] = pSound;
	}

}


void AudioSystem::UnLoadSound(const std::string& strSoundName)
{
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt == sgpImplementation->mSounds.end())
		return;

	AudioSystem::ErrorCheck(tFoundIt->second->release());
	sgpImplementation->mSounds.erase(tFoundIt);
}


int AudioSystem::PlaySound(const std::string& strSoundName, const Vector3x& vPosition, float fVolumedB)
{
	int nChannelId = sgpImplementation->mnNextChannelId++;
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt == sgpImplementation->mSounds.end())
	{
		Loadsound(strSoundName);
		tFoundIt = sgpImplementation->mSounds.find(strSoundName);
		if (tFoundIt == sgpImplementation->mSounds.end())
		{
			return nChannelId;
		}
	}
	FMOD::Channel* pChannel = nullptr;
	AudioSystem::ErrorCheck(sgpImplementation->mpSystem->playSound(tFoundIt->second, nullptr, true, &pChannel));
	if (pChannel)
	{
		FMOD_MODE currMode;
		tFoundIt->second->getMode(&currMode);
		if (currMode & FMOD_3D) 
		{
			FMOD_VECTOR position = VectorToFmod(vPosition);
			AudioSystem::ErrorCheck(pChannel->set3DAttributes(&position, nullptr));
		}
		AudioSystem::ErrorCheck(pChannel->setVolume((fVolumedB)));
		AudioSystem::ErrorCheck(pChannel->setPaused(false));
		sgpImplementation->mChannels[nChannelId] = pChannel;
	}
	return nChannelId;
}

void AudioSystem::StopAllChannels()
{

}


void AudioSystem::SetChanneledPosition(int nChannelId, const Vector3x& vPosition)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	FMOD_VECTOR position = VectorToFmod(vPosition);
	AudioSystem::ErrorCheck(tFoundIt->second->set3DAttributes(&position, NULL));
}


void AudioSystem::SetMusicvolume(int nChannelId, float fVolumedB)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	AudioSystem::ErrorCheck(tFoundIt->second->setVolume(fVolumedB));
}


void AudioSystem::SetSFXvolume(int nChannelId, float fVolumedB)
{
	for (auto aP = sgpImplementation->mChannels.begin(); aP != sgpImplementation->mChannels.end(); aP++)
	{
		if (aP->first != nChannelId)
		{
			AudioSystem::ErrorCheck(aP->second->setVolume(fVolumedB));
		}
	}
	/*auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	Audio_Engine::ErrorCheck(tFoundIt->second->setVolume(fVolumedB));*/
}


void AudioSystem::StopChannel(int ChannelID)
{
	/*auto tFoundIt = sgpImplementation->mChannels.find(ChannelID);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;
	Audio_Engine::ErrorCheck(tFoundIt->second->stop());*/

	for (auto tFoundIt : sgpImplementation->mChannels)
	{
		auto channel = tFoundIt.second;
		AudioSystem::ErrorCheck(channel->stop());
	}

}


void AudioSystem::PauseChannel(int ChannelID)
{
	/*cout << ChannelID << endl;
	auto tFoundIt = sgpImplementation->mChannels.find(ChannelID);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	Audio_Engine::ErrorCheck(tFoundIt->second->setPaused(true));*/

	for (auto tFoundIt : sgpImplementation->mChannels)
	{
		auto channel = tFoundIt.second;
		AudioSystem::ErrorCheck(channel->setPaused(true));
	}
}


void AudioSystem::ResumeChannel(int ChannelID)
{
	/*auto tFoundIt = sgpImplementation->mChannels.find(ChannelID);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	Audio_Engine::ErrorCheck(tFoundIt->second->setPaused(false));*/

	for (auto tFoundIt : sgpImplementation->mChannels)
	{
		auto channel = tFoundIt.second;
		AudioSystem::ErrorCheck(channel->setPaused(false));
	}
}


void AudioSystem::LoadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags) 
{
	auto tFoundIt = sgpImplementation->mBanks.find(strBankName);
	if (tFoundIt != sgpImplementation->mBanks.end())
		return;
	FMOD::Studio::Bank* pBank;
	AudioSystem::ErrorCheck(sgpImplementation->mpStudioSystem->loadBankFile(strBankName.c_str(), flags, &pBank));
	if (pBank) 
	{
		sgpImplementation->mBanks[strBankName] = pBank;
	}
}


void AudioSystem::LoadEvent(const std::string& strEventName) 
{
	auto tFoundit = sgpImplementation->mEvents.find(strEventName);
	if (tFoundit != sgpImplementation->mEvents.end())
		return;
	FMOD::Studio::EventDescription* pEventDescription = NULL;
	AudioSystem::ErrorCheck(sgpImplementation->mpStudioSystem->getEvent(strEventName.c_str(), &pEventDescription));
	if (pEventDescription) 
	{
		FMOD::Studio::EventInstance* pEventInstance = NULL;
		AudioSystem::ErrorCheck(pEventDescription->createInstance(&pEventInstance));
		if (pEventInstance) 
		{
			sgpImplementation->mEvents[strEventName] = pEventInstance;
		}
	}
}


void AudioSystem::PlayEvent(const std::string& strEventName) 
{
	auto tFoundit = sgpImplementation->mEvents.find(strEventName);
	if (tFoundit == sgpImplementation->mEvents.end()) 
	{
		LoadEvent(strEventName);
		tFoundit = sgpImplementation->mEvents.find(strEventName);
		if (tFoundit == sgpImplementation->mEvents.end())
			return;
	}
	tFoundit->second->setVolume(1.0f);
	tFoundit->second->start();
}


void AudioSystem::StopEvent(const std::string& strEventName, bool bImmediate) 
{
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end())
		return;

	FMOD_STUDIO_STOP_MODE eMode;
	eMode = bImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
	AudioSystem::ErrorCheck(tFoundIt->second->stop(eMode));
}


bool AudioSystem::IsEventPlaying(const std::string& strEventName) const 
{
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end())
		return false;

	FMOD_STUDIO_PLAYBACK_STATE* state = NULL;
	if (tFoundIt->second->getPlaybackState(state) == FMOD_STUDIO_PLAYBACK_PLAYING) 
	{
		return true;
	}
	return false;
}


void AudioSystem::GeteventParameter(const std::string& strEventName, const std::string& strParameterName, float* parameter) 
{
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end())
		return;

	FMOD::Studio::ParameterInstance* pParameter = NULL;
	AudioSystem::ErrorCheck(tFoundIt->second->getParameter(strParameterName.c_str(), &pParameter));
	AudioSystem::ErrorCheck(pParameter->getValue(parameter));
}


void AudioSystem::SetEventParameter(const std::string& strEventName, const std::string& strParameterName, float fValue) 
{
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end())
		return;

	FMOD::Studio::ParameterInstance* pParameter = NULL;
	AudioSystem::ErrorCheck(tFoundIt->second->getParameter(strParameterName.c_str(), &pParameter));
	AudioSystem::ErrorCheck(pParameter->setValue(fValue));
}


FMOD_VECTOR AudioSystem::VectorToFmod(const Vector3x& vPosition) 
{
	FMOD_VECTOR fVec;
	fVec.x = vPosition.x;
	fVec.y = vPosition.y;
	fVec.z = vPosition.z;
	return fVec;
}


float  AudioSystem::dbToVolume(float dB)
{
	return powf(10.0f, 0.05f * dB);
}


float  AudioSystem::VolumeTodb(float volume)
{
	return 20.0f * log10f(volume);
}


int AudioSystem::ErrorCheck(FMOD_RESULT result) 
{
	if (result != FMOD_OK) {
		std::cout << "FMOD ERROR " << result << std::endl;
		return 1;
	}
	// cout << "FMOD all good" << endl;
	return 0;
}


void AudioSystem::Shutdown() {
	delete sgpImplementation;
}


int AudioSystem::SendChannelId()
{
	return sgpImplementation->mnNextChannelId;
}


bool AudioSystem::IsPlaying(int ChannelID) const
{
	bool toReturn;
	sgpImplementation->mChannels.find(ChannelID)->second->isPlaying(&toReturn);
	return toReturn;
}
