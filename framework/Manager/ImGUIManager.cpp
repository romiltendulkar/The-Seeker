/**
 * @file ImGUIManager.cpp
 * @authors Pratyush Gawai
 * @date 10/2/19
 * @brief This file contains methods to draw Imgui debug display.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../Source/pch.h"
#include "ImGUIManager.h"
#include "TimerManager.h"
#include "ResourceManager.h"
#include "MemoryManager.h"
#include "CollisionManager.h"
#include "ObjectFactoryManager.h"
#include "DXInputManager.h"

#include "..\Source\Entity\Player.h"

ImGUIManager::ImGUIManager()
{
	english = true;
	portuguese = false;
	filename = "yababeh";
}

ImGUIManager::~ImGUIManager()
{
}

ImGUIManager * ImGUIManager::GetInstance()
{
	if (!mInstance)
	{
		mInstance = new ImGUIManager();
	}
	return mInstance;
}

void ImGUIManager::Initialize(HWND hwnd, ID3D11Device * device, ID3D11DeviceContext * context)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	
	io = ImGui::GetIO();
	(void)io;
	
	//io.Fonts->AddFontFromFileTTF("myfontfile.ttf", size_in_pixels, NULL, io.Fonts->GetGlyphRangesJapanese());
	io.ConfigWindowsMoveFromTitleBarOnly = false;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
   //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

   // Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, context);

	show_demo_window = false;
	show_another_window = false;
}

void ImGUIManager::Destroy()
{
	//CleanUP Imgui stuff
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	delete mInstance;
}

void ImGUIManager::Update(float rdeltaTime)
{

}

void ImGUIManager::DrawWindows()
{
	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	//// Start the Dear ImGui frame
	//ImGui_ImplDX11_NewFrame();
	//ImGui_ImplWin32_NewFrame();
	//ImGui::NewFrame();

	//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	////if (show_demo_window)
	//	//ImGui::ShowDemoWindow(&show_demo_window);

	//DrawDebugWindows();
	//DrawLevelEditorWindow();
	//// Rendering
	//ImGui::Render();

	//ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

}

void ImGUIManager::HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}
void ImGUIManager::DrawDebugWindows()
{

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	//ImGui::SetNextWindowSize(ImVec2(300, 100),0);
	ImGui::Begin(u8"Debug Stats");
	
	//level editor on
	static int clicked = 0;
	/*if (ImGui::Button(u8"LevelEditor"))
		clicked++;
	if (clicked & 1)
	{
		LevelEditor::GetInstance()->levelEditoris_on = true;
		ImGui::SameLine();
		ImGui::Text(u8"Level Editor is on!");
	}
	else
	{
		LevelEditor::GetInstance()->levelEditoris_on = false;
	}
	static int clickedt = 0;
	if (LevelEditor::GetInstance()->levelEditoris_on == true) {
		if (ImGui::Button(u8"Type I to close"))
		{
			clickedt++;
			DXInputManager::GetInstance()->AcquireKeybroad();
		}
			
		if (clickedt & 1)
		{
			LevelEditor::GetInstance()->toggleis_on = true;
			ImGui::SameLine();
			ImGui::Text(u8"Now u can type!");
		}
		else
		{
			LevelEditor::GetInstance()->toggleis_on = false;
		}
	}*/
	//filename input
	/*if (LevelEditor::GetInstance()->levelEditoris_on == true)
	{
		static char str1[256] = "";
		ImGui::InputTextWithHint("", "enter file name here", str1, IM_ARRAYSIZE(str1));  
		ImGui::SameLine(); HelpMarker("When level editor is on type the file name and click on s to save the file");
		ImGui::SameLine();
		if (ImGui::Button("Save"))
		{
			ObjectFactoryManager::GetInstance()->SaveLevel();
		}
		filename = str1;
	}*/
	//FrameRate
	if (ImGui::CollapsingHeader(u8"Performance"))
	{
		ImGui::Text("FPS: %f", 1 / TimerManager::GetInstance()->getDeltaTime());
	}

	if (ImGui::CollapsingHeader(u8"Mouse pos"))
	{
		ImGui::Text("xyz: %f %f %f", PhysicsManager::GetInstance()->ppoint.x, PhysicsManager::GetInstance()->ppoint.y, PhysicsManager::GetInstance()->ppoint.z);
	}
	//Resources
	if (ImGui::CollapsingHeader(u8"Resources"))
	{
		//JSON files
		if (ImGui::TreeNode(u8"JSON Files"))
		{
			for (auto& jsons : ResourceManager::GetInstance()->LJsonData)
			{
				unsigned int numObj = MemoryManager::GetInstance()->current_position;
				if (ImGui::Button(jsons.first.c_str()) )
				{
					LevelEditor::GetInstance()->createObject(jsons.first.c_str(),numObj);
				}
			}
			ImGui::TreePop();
		}
		//Shapes
		if (ImGui::TreeNode(u8"Shapes"))
		{
			//unsigned int numObj = MemoryManager::GetInstance()->current_position;
			for (auto& shape : ResourceManager::GetInstance()->LShape)
			{
				unsigned int numObj = MemoryManager::GetInstance()->current_position;
				if (ImGui::Button(shape.first.c_str()) )
				{
					LevelEditor::GetInstance()->createObject(shape.first.c_str(),  numObj);
				}
			}
			ImGui::TreePop();
		}

	}

	if (ImGui::CollapsingHeader(u8"Objects"))
	{
		bool isclicked = false;
		unsigned int numObj = MemoryManager::GetInstance()->current_position;
		ImGui::Text(u8"Number of GameObjects : %d", numObj);
		int csize = static_cast<int>(CollisionManager::GetInstance()->mContacts.size());
		ImGui::Text(u8"Number of collisions : %d", csize);

		for (unsigned int i = 0; i < numObj; ++i)
		{
			if((*MemoryManager::GetInstance()->ObjectPool)[i].pBd)
				(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->objectSelected = false;
			//if (ImGui::TreeNode((void*)(intptr_t)i, "Object %d", i))
			std::string name = (*MemoryManager::GetInstance()->ObjectPool)[i].pEn->mEntityName;
		
			if (ImGui::TreeNode((void*)(intptr_t)i, "Object %d : %s", i, name.c_str()))
			{
	
				
				if (LevelEditor::GetInstance()->levelEditoris_on == true)
				{
					if ((*MemoryManager::GetInstance()->ObjectPool)[i].pBd)
						(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->objectSelected = true;
										
				}
				//Transform component
				if ((*MemoryManager::GetInstance()->ObjectPool)[i].pTr)
				{
					
					
					//LevelEditor::GetInstance()->hightlightObject(*MemoryManager::GetInstance()->ObjectPool)[i].pTr);
					if (ImGui::TreeNode(u8"Transform"))
					{
						Transform* pTr = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr;
						//Position
						float pos[3] = { pTr->mPositionX,pTr->mPositionY,pTr->mPositionZ };
						//ImGui::InputFloat3("Position", pos);
						ImGui::InputFloat(u8"Posn X", &(*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mPositionX, 0.1f, 1.0f, "%.3f");
						ImGui::InputFloat(u8"Posn Y", &(*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mPositionY, 0.1f, 1.0f, "%.3f");
						ImGui::InputFloat(u8"Posn Z", &(*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mPositionZ, 0.1f, 1.0f, "%.3f");

						if ((*MemoryManager::GetInstance()->ObjectPool)[i].pBd)
						{
							(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mPos_x = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mPositionX;
							(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mPos_y = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mPositionY;
							(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mPos_z = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mPositionZ;
						}
						
						/*if (pTr->mPositionX != pos[0] || pTr->mPositionY != pos[1] || pTr->mPositionZ != pos[2])
						{
							pTr->mPositionX = pos[0];
							pTr->mPositionY = pos[1];
							pTr->mPositionZ = pos[2];
						}*/
						//Scale
						float scale[3] = { pTr->mScaleX,pTr->mScaleY,pTr->mScaleZ };
						ImGui::InputFloat(u8"Scalen X", &(*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mScaleX, 0.1f, 1.0f, "%.3f");
						ImGui::InputFloat(u8"Scalen Y", &(*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mScaleY, 0.1f, 1.0f, "%.3f");
						ImGui::InputFloat(u8"Scalen Z", &(*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mScaleZ, 0.1f, 1.0f, "%.3f");
						if ((*MemoryManager::GetInstance()->ObjectPool)[i].pBd)
						{
							(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mScale_x = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mScaleX;
							(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mScale_y = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mScaleY;
							(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mScale_z = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mScaleZ;
						}

						ImGui::TreePop();
					}
				}
				//Asset Component
				if ((*MemoryManager::GetInstance()->ObjectPool)[i].pAs)
				{
					if (ImGui::TreeNode(u8"Asset"))
					{
						Asset* pAs = (*MemoryManager::GetInstance()->ObjectPool)[i].pAs;
						if (pAs->ModelName != "")//if model
							ImGui::Text(pAs->ModelName.c_str());
						else
						ImGui::Text(pAs->ShapeName.c_str()); //else shape
						

						ImGui::TreePop();
					}
				}
				//Body Component
				if ((*MemoryManager::GetInstance()->ObjectPool)[i].pBd)
				{
					if (ImGui::TreeNode(u8"Body"))
					{
						Body* pBd = (*MemoryManager::GetInstance()->ObjectPool)[i].pBd;

						//Mass
						float m = pBd->mMass;
						ImGui::InputFloat(u8"Mass", &m);
						//Position
						float pos[3] = { pBd->mPos_x,pBd->mPos_y,pBd->mPos_z };
						if (pBd->mShape->type == SHAPE_AABB)
						{
							ShapeAABB* AABB = static_cast<ShapeAABB*>(pBd->mShape);

							float top = AABB->mTop;
							float bot = AABB->mBottom;
							float left = AABB->mLeft;
							float right = AABB->mRight;
							float in = AABB->mIn;
							float out = AABB->mOut;

							float tb[2] = { top,bot };
							float lr[2] = { left,right };
							float io[2] = { in,out };

							ImGui::InputFloat2("Top & Bot ", tb);
							ImGui::InputFloat2("Left & Right ", lr);
							ImGui::InputFloat2("In & Out ", io);

						}
						ImGui::InputFloat3(u8"Position ", pos);
						//Acceleration
						float acc[3] = { pBd->mAcc_x,pBd->mAcc_y,pBd->mAcc_z };
						ImGui::InputFloat3(u8"Acceleration ", acc);
						//Velocity
						float vel[3] = { pBd->mVelo_x,pBd->mVelo_y,pBd->mVelo_z };
						ImGui::InputFloat3(u8"Velocity ", vel);


						ImGui::TreePop();
					}
				}

				//Controller component
				if ((*MemoryManager::GetInstance()->ObjectPool)[i].pCr)
				{
					ImGui::Text(u8"Controller");
				}
				//Animation Component
				if ((*MemoryManager::GetInstance()->ObjectPool)[i].pAn)
				{
					if (ImGui::TreeNode(u8"Animation"))
					{
						Animation* pAn = (*MemoryManager::GetInstance()->ObjectPool)[i].pAn;

						ImGui::Text(u8"Current animation : %s", pAn->GetCurrentStatus().c_str());

						ImGui::Text(u8"Animation speed : % f  ", pAn->GetSpeed());

						ImGui::Text(u8"Animation Total time : %f ", pAn->GetTotalTime());

						ImGui::TreePop();
					}

				}
				ImGui::TreePop();
			}
			

		}
	}

	ImGui::End();
}

void ImGUIManager::DrawLevelEditorWindow()
{
	ImGui::SetNextWindowPos(ImVec2(1500, 0));
	ImGui::SetNextWindowSize(ImVec2(400, 400),0);
	ImGui::Begin(u8"Level Editor");

	int reindex;
	
	int no_ens = ResourceManager::GetInstance()->LJsonData.size() - 1;//lvl file -1 = no of entities in the thing

	if (ImGui::CollapsingHeader(u8"Objects"))
	{
		ImGui::Text(u8"Number of Entities available in the file: %d", no_ens);
		int ct = 0;
		for (auto objs : ObjectFactoryManager::GetInstance()->LEntityNames)
		{
			if (ct != 0)
			{
				if (ImGui::Button(objs.c_str()))
				{
					reindex = ObjectFactoryManager::GetInstance()->CreateRunTimeObject(objs.c_str());
					
					//if (ImGui::TreeNode(u8"Transform"))
					//{

					//	Transform* pTr = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr;
					//	//Position
					//	float pos[3] = { pTr->mPositionX,pTr->mPositionY,pTr->mPositionZ };
					//	//ImGui::InputFloat3("Position", pos);
					//	ImGui::InputFloat(u8"Posn X", &(*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mPositionX, 0.1f, 1.0f, "%.3f");
					//	ImGui::InputFloat(u8"Posn Y", &(*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mPositionY, 0.1f, 1.0f, "%.3f");
					//	ImGui::InputFloat(u8"Posn Z", &(*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mPositionZ, 0.1f, 1.0f, "%.3f");

					//	if ((*MemoryManager::GetInstance()->ObjectPool)[i].pBd)
					//	{
					//		(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mPos_x = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mPositionX;
					//		(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mPos_y = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mPositionY;
					//		(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mPos_z = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mPositionZ;
					//	}

					//	/*if (pTr->mPositionX != pos[0] || pTr->mPositionY != pos[1] || pTr->mPositionZ != pos[2])
					//	{
					//		pTr->mPositionX = pos[0];
					//		pTr->mPositionY = pos[1];
					//		pTr->mPositionZ = pos[2];
					//	}*/
					//	//Scale
					//	float scale[3] = { pTr->mScaleX,pTr->mScaleY,pTr->mScaleZ };
					//	ImGui::InputFloat(u8"Scalen X", &(*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mScaleX, 0.1f, 1.0f, "%.3f");
					//	ImGui::InputFloat(u8"Scalen Y", &(*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mScaleY, 0.1f, 1.0f, "%.3f");
					//	ImGui::InputFloat(u8"Scalen Z", &(*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mScaleZ, 0.1f, 1.0f, "%.3f");
					//	if ((*MemoryManager::GetInstance()->ObjectPool)[i].pBd)
					//	{
					//		(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mScale_x = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mScaleX;
					//		(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mScale_y = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mScaleY;
					//		(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mScale_z = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mScaleZ;
					//	}

					//	ImGui::TreePop();
					//}
					
					
				}
			}
			ct++;
			
		}
	}
	
		
	std::vector<int>* co = &ObjectFactoryManager::GetInstance()->LDynamicList;
	if (ImGui::CollapsingHeader(u8"Created Objects"))
	{
		for (int x = 0; x < co->size(); ++x)
		{
			int i = co->at(x);
			std::string name = (*MemoryManager::GetInstance()->ObjectPool)[i].pEn->mEntityName;

			if (ImGui::TreeNode((void*)(intptr_t)i, "Created Object %d : %s", x, name.c_str()))
			{

				if (ImGui::TreeNode(u8"Transform"))
				{

					Transform* pTr = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr;
					//Position
					float pos[3] = { pTr->mPositionX,pTr->mPositionY,pTr->mPositionZ };
					//ImGui::InputFloat3("Position", pos);
					ImGui::InputFloat(u8"Posn X", &(*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mPositionX, 0.1f, 1.0f, "%.3f");
					ImGui::InputFloat(u8"Posn Y", &(*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mPositionY, 0.1f, 1.0f, "%.3f");
					ImGui::InputFloat(u8"Posn Z", &(*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mPositionZ, 0.1f, 1.0f, "%.3f");

					if ((*MemoryManager::GetInstance()->ObjectPool)[i].pBd)
					{
						(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mPos_x = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mPositionX;
						(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mPos_y = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mPositionY;
						(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mPos_z = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mPositionZ;
					}


					//Scale
					float scale[3] = { pTr->mScaleX,pTr->mScaleY,pTr->mScaleZ };
					ImGui::InputFloat(u8"Scalen X", &(*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mScaleX, 0.1f, 1.0f, "%.3f");
					ImGui::InputFloat(u8"Scalen Y", &(*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mScaleY, 0.1f, 1.0f, "%.3f");
					ImGui::InputFloat(u8"Scalen Z", &(*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mScaleZ, 0.1f, 1.0f, "%.3f");
					if ((*MemoryManager::GetInstance()->ObjectPool)[i].pBd)
					{
						(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mScale_x = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mScaleX;
						(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mScale_y = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mScaleY;
						(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mScale_z = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mScaleZ;

						

						if ((*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mShape->type == SHAPE_AABB)
						{
							ShapeAABB* newShape = static_cast<ShapeAABB*>((*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mShape);

							newShape->mBottom = (0 - (*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mScale_z)/2;
							newShape->mTop = (0 + (*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mScale_z)/2;

							newShape->mIn = (0 + (*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mScale_y)/2;
							newShape->mOut = (0 - (*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mScale_y)/2;

							newShape->mLeft = (0 - (*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mScale_x)/2;
							newShape->mRight = (0 + (*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mScale_x)/2;
						}

					}

					ImGui::TreePop();
				}
				
				ImGui::TreePop();
			}

		}
	}

	ImGui::Text(u8"Selected Object key value : %d", selectedkey);

	if (ImGui::Button(u8"Save Level"))
		ObjectFactoryManager::GetInstance()->SaveLevel();
	
	ImGui::End();
}

void ImGUIManager::Resize(int width, int height)
{
	this->width = width;
	this->height = height;
}

void ImGUIManager::DrawCharStats()
{
	Player* pPlayer = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(400, 400));
	//if (japanese)
	//{
	//	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 20.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//}
	ImGui::Begin("Character Stats");
	if (ImGui::Button(u8"English"))
	{
		english = true;
		portuguese = false;
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"Portuguese"))
	{
		english = false;
		portuguese = true;
	}
	
	if (english) {
		ImGui::Text(u8"HP %d/100", (int)pPlayer->health);
		ImGui::Text(u8"Base DMG %.1f", pPlayer->mAttack);
		ImGui::Text(u8"Base DEF %.1f", pPlayer->mDefense);
		if (pPlayer->Equipped == Player::Weapons::Wooden_Sword)
		{
			ImGui::Text("Equipped Melee: Fist");
		}
		if (pPlayer->Equipped == Player::Weapons::Iron_Sword)
		{
			ImGui::Text(u8"Equipped Melee: Gauntlet");
		}
		if (pPlayer->REquipped == Player::Ranged_Weapons::Slingshot)
		{
			ImGui::Text(u8"Equipped Ranged: Slingshot");
		}
		if (pPlayer->REquipped == Player::Ranged_Weapons::Fireball)
		{
			ImGui::Text(u8"Equipped Ranged: Fireball");
		}
		//ImGui::Text("\nPress Q to change Melee");
		//ImGui::Text("\nPress E to change Ranged");
		if (DXInputManager::GetInstance()->isKeyTrigger(DIK_Q) == true)
		{
			if (pPlayer->Equipped == Player::Weapons::Wooden_Sword)
			{
				pPlayer->Equipped = Player::Weapons::Iron_Sword;
			}
			if (pPlayer->Equipped == Player::Weapons::Iron_Sword)
			{
				pPlayer->Equipped = Player::Weapons::Wooden_Sword;
			}
		}
		if (DXInputManager::GetInstance()->isKeyTrigger(DIK_E) == true)
		{
			if (pPlayer->REquipped == Player::Ranged_Weapons::Fireball)
			{
				pPlayer->REquipped = Player::Ranged_Weapons::Slingshot;
			}
			if (pPlayer->REquipped == Player::Ranged_Weapons::Slingshot)
			{
				pPlayer->REquipped = Player::Ranged_Weapons::Fireball;
			}
		}
	}
	if (portuguese)
	{
		ImGui::Text(u8"HP %d/100", (int)pPlayer->health);
		ImGui::Text(u8"Dano base %.1f", pPlayer->mAttack);
		ImGui::Text(u8"Dano base %.1f", pPlayer->mDefense);
		if (pPlayer->Equipped == Player::Weapons::Wooden_Sword)
		{
			ImGui::Text("Corpo a corpo equipado : Primeiro");
		}
		if (pPlayer->Equipped == Player::Weapons::Iron_Sword)
		{
			ImGui::Text(u8"Corpo a corpo equipado : Manopla");
		}
		if (pPlayer->REquipped == Player::Ranged_Weapons::Slingshot)
		{
			ImGui::Text(u8"Equipado à distância: Estilingue");
		}
		if (pPlayer->REquipped == Player::Ranged_Weapons::Fireball)
		{
			ImGui::Text(u8"Equipado à distância: bola de fogo");
		}
		//ImGui::Text("\nPress Q to change Melee");
		//ImGui::Text("\nPress E to change Ranged");
		if (DXInputManager::GetInstance()->isKeyTrigger(DIK_Q) == true)
		{
			if (pPlayer->Equipped == Player::Weapons::Wooden_Sword)
			{
				pPlayer->Equipped = Player::Weapons::Iron_Sword;
			}
			if (pPlayer->Equipped == Player::Weapons::Iron_Sword)
			{
				pPlayer->Equipped = Player::Weapons::Wooden_Sword;
			}
		}
		if (DXInputManager::GetInstance()->isKeyTrigger(DIK_E) == true)
		{
			if (pPlayer->REquipped == Player::Ranged_Weapons::Fireball)
			{
				pPlayer->REquipped = Player::Ranged_Weapons::Slingshot;
			}
			if (pPlayer->REquipped == Player::Ranged_Weapons::Slingshot)
			{
				pPlayer->REquipped = Player::Ranged_Weapons::Fireball;
			}
		}
	}
	ImGui::End();

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGUIManager::DrawCompletion()
{

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Finish");

	ImGui::Text(u8"Thank You for Playing");



	ImGui::End();

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGUIManager::NeedCompletion()
{
	

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("OOPS");

	ImGui::Text(u8"Need all Collectibles to Unlock");

	ImGui::End();

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}