/**
 * @file Animation.cpp
 * @authors Ching-Yen Lin.
 * @date 9/25/19
 * @brief Animation component file.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#include "../pch.h"
#include "Animation.h"
#include "../GameObject.h"
#include "../../Manager/MemoryManager.h"
#include "../Graphics/Shape.h"
//#include "../../Manager/ScriptManager.h"
Animation::Animation() : Component(ANIMATION)
{
	mTotalTime = 0.0f;
	mCurrentStatus = "";
	mSpeed=0.0f;
	misLoop=false;
	key = -1;
}


Animation::~Animation()
{
	LAnimationData.clear();
	while(LAnimationChain.empty())
	{
		LAnimationChain.pop();
	}
}

void Animation::Update(float deltaTime)
{
	mTotalTime += deltaTime;
	//if (mPreName != LAnimationChain.front().AnimationName)
	//	mTimer += deltaTime;
	//cout << mPreName <<""<< LAnimationChain.front().AnimationName <<" "<< mTimer/ mChangeTime << endl;
	if (LAnimationChain.size() > 1)
	{
		mTimer += deltaTime;
		if (mTimer / mChangeTime > 1)
		{
			LAnimationChain.pop();
		}
	}
	if (LAnimationChain.size() > 2)
	{
		auto a = LAnimationChain.front();
		queue<AnimationData>empty;
		swap(LAnimationChain, empty);
		LAnimationChain.push(a);
	}
	//if (LAnimationChain.size() > 2)
	//{
	//	system("pause");
	//}
	//cout << mTimer / mChangeTime << endl;
	while (LAnimationChain.size())
	{
		if (LAnimationChain.back().AnimationTotalTime/ LAnimationChain.back().AnimationSpeed > mTotalTime)
		{
			mCurrentStatus = LAnimationChain.back().AnimationName;
			mSpeed = LAnimationChain.back().AnimationSpeed;
			return;
		}
		//if (misLoop)
		{
			LAnimationChain.push(LAnimationChain.back());
		}
		LAnimationChain.pop();
		mTotalTime = 0;
	}
	mTotalTime = 0;
	mCurrentStatus = "";



}
void Animation::Serialize(Json::Value root, unsigned int key)
{
	/*sol::state state;
	state.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string);
	state.script_file("Source/Script/Animation.lua");
	string animation = state["config"]["animation"];*/

	//initial like this
	
	mSpeed = 1.0f;
	misLoop = false;
	//lAnimationData.push_back({ "Armature|start", 1.0, 8.0f / 24.0f });
	//lAnimationData.push_back({ "Armature|walk", 1.0, 7.0f / 24.0f });
	//lAnimationData.push_back({ "Armature|stop", 1.0, 30.0f / 24.0f });
	Json::Value objectRoot = root["AnimationList"];
	for (unsigned int i = 0; i < objectRoot.size(); i++)
	{
		LAnimationData.push_back({ objectRoot[i][0].asString(),
			objectRoot[i][1].asFloat(),
			objectRoot[i][2].asFloat() / objectRoot[i][3].asFloat() });
	}
	objectRoot = root["AnimationAddtionalList"];
	for (unsigned int i = 0; i < objectRoot.size(); i++)
	{
		FBX* pFBX = static_cast<FBX*>((*MemoryManager::GetInstance()->ObjectPool)[key].pAs->tshape);
		pFBX->AddAnimationFromFile("./Resource/Model/" + objectRoot[i][0].asString(), objectRoot[i][1].asString());
		LAnimationData.push_back({ objectRoot[i][1].asString(),
			objectRoot[i][2].asFloat(),
			objectRoot[i][3].asFloat() / objectRoot[i][4].asFloat() });
	}

	LAnimationChain.push({ "idle", 1.0, 59 / 30.0f });


//lAnimationChain.push({ "run", 1.0, 59 / 30.0f });
//lAnimationChain.push({ "run", 1.0, 19 / 30.0f });
//lAnimationChain.push({ "roll", 1.0, 1 / 30.0f });
	//lAnimationChain.push({ "idle", 1.0, 59 / 30.0f });
	//lAnimationChain.push({ "hatk", 1.0, 31 / 30.0f });
	//lAnimationChain.push({ "vatk", 1.0, 30 / 30.0f });
	//summe there is animation chain
	/*lAnimationChain.push({ "Armature|start", 1.0, 8.0f / 24.0f });
	lAnimationChain.push({ "Armature|walk", 1.0, 30.0f / 24.0f });
	lAnimationChain.push({ "Armature|walk", 1.0, 30.0f / 24.0f });
	lAnimationChain.push({ "Armature|walk", 1.0, 30.0f / 24.0f });
	lAnimationChain.push({ "Armature|walk", 1.0, 30.0f / 24.0f });
	lAnimationChain.push({ "Armature|stop", 1.0, 7.0f / 24.0f });*/
		
	
	(*MemoryManager::GetInstance()->ObjectPool)[key].pAn = this;
	(*MemoryManager::GetInstance()->ObjectPool)[key].pAn->mpEnowner = (*MemoryManager::GetInstance()->ObjectPool)[key].pEn;
	
	this->key = key;
}

void Animation::DeSerialize(Json::Value& root)
{
	root["Animation"] = (Json::objectValue);
}

///**
//  This function takes in name of the animation and its time creates a small animation chain
//*/
//
//void Animation::ChangeAnimation(unsigned int walks, std::string A_start, std::string A_walk, std::string A_stop, float speed, float time_start, float time_walk, float time_stop)
//{
//	while (!LAnimationChain.empty())
//	{
//		LAnimationChain.pop();
//	}
//	misLoop = false;
//	LAnimationChain.push({ A_start, speed, time_start });
//	for (unsigned int i = 0; i < walks; ++i)
//	{
//		LAnimationChain.push({ A_walk, speed, 30.0f / 24.0f });
//	}
//	LAnimationChain.push({ A_stop, speed, time_stop });
//}

///**
//   This function takes in name of the animation and its time and stops the animation
//*/
//void Animation::ChangeAnimationStop(std::string A_stop, float speed, float time)
//{
//
//	while (!LAnimationChain.empty())
//	{
//		LAnimationChain.pop();
//	}
//	LAnimationChain.push({ A_stop, speed, time });
//	misLoop = false;
//}



void Animation::SetCurrentStatus(string name, float speed, float time)
{
	
	if (!LAnimationChain.empty())
	{
		if (LAnimationChain.front().AnimationName == name)
		{
			if (LAnimationChain.front().AnimationSpeed != speed || LAnimationChain.front().AnimationTotalTime != time)
			{
				LAnimationChain.front().AnimationSpeed = speed;
				LAnimationChain.front().AnimationTotalTime = time;
				mTotalTime = 0.0f;
			}
			return;
		}
	}
	mTotalTime = 0.0f;
	queue<AnimationData>empty;
	swap(LAnimationChain, empty);
	AddAnimationChain(name,  speed,  time);
}

void Animation::ChangeCurrentStatus(string name,float speed,float time)
{
	//if (!LAnimationChain.empty())
	//{
	//	if (LAnimationChain.front().AnimationName != mPreName)
	//	{
	//		mPreName = LAnimationChain.front().AnimationName;
	//		mPreTime = mTotalTime;
	//		mTimer = 0;
	//	}
	//}
	//SetCurrentStatus(name, speed, time);
	//if (mPreName == LAnimationChain.front().AnimationName)
	//{
	//	mTimer = mChangeTime;
	//}
	/*return;*/
	if (!LAnimationChain.empty())
	{
		if (LAnimationChain.front().AnimationName == name)
		{
			misLoop = true;
			return;
		}
		if (LAnimationChain.back().AnimationName == name)
		{
			misLoop = false;
			return;
		}
	}
	AddAnimationChain(name, speed, time);
	mTotalTime = 0.0f;
	if (LAnimationChain.size() > 1)
	{
		mTimer = 0;
	}
}

void Animation::AddAnimationChain(string name, float speed, float time)
{
	for (size_t i = 0; i < LAnimationData.size(); i++)
	{
		if (LAnimationData[i].AnimationName == name)
		{
			if (speed == 0 || time == 0)
			{
				LAnimationChain.push(LAnimationData[i]);
			}
			else
			{
				LAnimationChain.push({ name, speed, time });
			}
			break;
		}
	}
}

float Animation::GetTotalTime()
{
	return mTotalTime;
}

string Animation::GetCurrentStatus()
{

	return mCurrentStatus;
}

float Animation::GetSpeed()
{
	return mSpeed;
}

bool Animation::IsLoop()
{
	return misLoop;
}

float Animation::GetBlendingValue()
{
	if (mTimer / mChangeTime > 1)
	{
		return 1;
	}
	return mTimer / mChangeTime;
}

queue<AnimationData>& Animation::GetAnimationQueue()
{
	return LAnimationChain;
}

void Animation::HandleEvent(Event *EventToHandle)
{

}
