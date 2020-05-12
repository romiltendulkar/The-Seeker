#include "../pch.h"
#include "Charger.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"

#include "Player.h"
#include "..\Events\HitEvent.h"
#include "..\Events\EffectDamage.h"
#include "../../Manager/ParticleManager.h"

#include "../Events/CollisionEvent.h"
#include "..\..\Manager\EventManager.h"

Charger::Charger() : Entity(ENTITY_TYPE::CHARGER)
{
	mAffectedBy = nullptr;
}

Charger::~Charger() {}

void Charger::Serialize(Json::Value root, unsigned int position)
{

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	Json::Value dataRoot = root["Transform"];

	//Transform
	if (!dataRoot.isNull())
	{
		Transform* pTr = new Transform();
		pTr->Serialize(dataRoot, position);
	}
	dataRoot = root["Sleep"];
	if (!dataRoot.isNull())
	{
		sleep = dataRoot.asBool();
	}
	//Asset
	dataRoot = root["Asset"];

	if (!dataRoot.isNull())
	{
		Asset* pAs = new Asset();
		pAs->Serialize(dataRoot, position);
	/*	pAs->obj->SetDiffuse(0.0f, 1.0f, 0.0f);*/
	}

	//Body
	dataRoot = root["Body"];

	if (!dataRoot.isNull())
	{
		Body* pBd = new Body();
		pBd->Serialize(dataRoot, position);
		this->pBd = pBd;
	}

	//Animation
	dataRoot = root["Animation"];

	if (!dataRoot.isNull())
	{
		Animation* pAn = new Animation();
		pAn->Serialize(dataRoot, position);
		this->pAn = pAn;
	}

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Charger";
	key = position;
	this->complete = true;
	this->runtime = false;
	ObjectFactoryManager::GetInstance()->LoadObject("Guidebox", key + 1);
	guide_key = key + 1;
	(*MemoryManager::GetInstance()->ObjectPool)[guide_key].pBd->mPos_x = 25;
	(*MemoryManager::GetInstance()->ObjectPool)[guide_key].pBd->mPos_y = 25;
	(*MemoryManager::GetInstance()->ObjectPool)[guide_key].pBd->mPos_z = 8;
	ObjectFactoryManager::GetInstance()->LoadObject("Visible_Box", key + 2);
	(*MemoryManager::GetInstance()->ObjectPool)[key + 2].pBd->mPos_x = pBd->mPos_x;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 2].pBd->mPos_y = pBd->mPos_y;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 2].pBd->mPos_z = pBd->mPos_z - 100;
	if (sleep)
	{
		health = 5;
	}
	else
	{
		health = 500;
	}
	ObjectFactoryManager::GetInstance()->LoadObject("HealthBar", key+3);
	ohealth = health;
}
void Charger::Update(float deltaTime)
{
	if (got_hit_counter > 0)
	{
		got_hit_counter--;
	}
	Transform* pTr = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 3].pTr->mPositionX = pTr->mPositionX;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 3].pTr->mPositionY = pTr->mPositionY;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 3].pTr->mPositionZ = pTr->mPositionZ + 5;
	(*MemoryManager::GetInstance()->ObjectPool)[key + 3].pTr->mScaleX = 2 * (health / 500);
	if (health > 0)
	{
		
		if (sleep)
		{
			Transform* pTr = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr;
			{
				Particle temp;
				temp.age = 2.0;
				temp.position = Vector4(pTr->mPositionX, pTr->mPositionY, pTr->mPositionZ, 0.3);
				temp.TextureIndex = 2;
				temp.velocity = Vector4(0.7, 0.7, 0.7, 0.3);
				ParticleManager::GetInstance()->TimerEmit(60, temp);
			}
			return;
		}
		floor_scale_x = ObjectFactoryManager::GetInstance()->floor_scale_x;
		floor_scale_y = ObjectFactoryManager::GetInstance()->floor_scale_y;
		floor_x = ObjectFactoryManager::GetInstance()->floor_x;
		floor_y = ObjectFactoryManager::GetInstance()->floor_y;
		////create the grid from the floor position.
		//map = new grid *[floor_scale_x];
		//for (int i = 0; i < floor_scale_y; ++i)//left to right
		//{
		//	map[i] = new grid[floor_scale_y];
		//}
		//for (int i = 0; i < floor_scale_x; ++i)
		//{
		//	for (int j = 0; j < floor_scale_y; ++j)
		//	{
		//		map[i][j] = { i,j,false };
		//	}
		//}

		//std::vector<unsigned int>::iterator itr = ObjectFactoryManager::GetInstance()->LWallPositions.begin();
		//while (itr != ObjectFactoryManager::GetInstance()->LWallPositions.end())
		//{
		//	int i = (*MemoryManager::GetInstance()->ObjectPool)[(*itr)].pBd->mPos_x;
		//	int j = (*MemoryManager::GetInstance()->ObjectPool)[(*itr)].pBd->mPos_y;
		//	float scale_i = (*MemoryManager::GetInstance()->ObjectPool)[(*itr)].pBd->mScale_x;
		//	float scale_j = (*MemoryManager::GetInstance()->ObjectPool)[(*itr)].pBd->mScale_y;
		//	i = i - scale_i / 2;//leftmost
		//	j = j - scale_j / 2;//bottom-most
		//	for (int y = j; y < j + scale_j; ++y)
		//	{
		//		for (int x = i; x < i + scale_i; ++x)
		//		{
		//			(map[y][x]).is_wall = true;
		//		}
		//	}
		//	++itr;
		//}
		/*std::cout << std::endl;
		for (int j = floor_scale_y - 1; j > 0; --j)
		{
			for (int i = 0; i < floor_scale_x; ++i)
			{
				std::cout << map[j][i].is_wall << "   ";
			}
			std::cout << std::endl << std::endl;
		}*/
		//implement A* here to confirm that it even works.
		auto map = MemoryManager::GetInstance()->map;
		std::vector<res_pair>result;
		std::vector<node> neighbors;

		player_x = (int)(*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_x;
		player_y = (int)(*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_y;
		++frame_counter;
		float distance = sqrt((player_x - pBd->mPos_x) * (player_x - pBd->mPos_x) + (player_y - pBd->mPos_y) * (player_y - pBd->mPos_y));
		if (distance > 20)
		{
			pBd->mVelo_x = pBd->mVelo_y = 0;
			wentin = 0;
		}
		//if (frame_counter % 120 == 0)
		//{
		//	//std::cout << "lol" << std::endl;
		//}
		auto proxy_map= MemoryManager::GetInstance()->proxy_map;
		//proxy_map = new node *[floor_scale_x];
#pragma omp parallel for schedule(dynamic, 1)
		for (int i = 0; i < floor_scale_y; ++i)//left to right
		{
			/*proxy_map[i] = new node[floor_scale_y];*/
			std::fill(proxy_map[i], proxy_map[i] + int(floor_scale_y), node());
		}
		
		if (got_hit_counter == 0)
		{
			if (frame_counter % 120 == 0 && distance <= 20)
			{

				wentin = 1;
				//std::cout << map[player_x][player_y].is_wall;
				openlist.clear();
				index_offset = 2;
				//map done
				int iter = 0;
				node start;
				int goal_x = player_x, goal_y = player_y;
				start.gx = 0;//starting node has 0 weight.
				float h = Octile(player_x, player_y, goal_x, goal_y);//f = g + h, h = heuristic.
				start.hx = h;
				start.fx = start.hx;//^^
				start.x = (int)this->pBd->mPos_x;//
				start.y = (int)this->pBd->mPos_y;//
				start_x = start.x;
				start_y = start.y;
				start.parent_x = -1;//has no parent, that is what 999 signifies.
				start.parent_y = -1;
				start.open_list = 1;//its on the p=open list
				start.closed_list = 0;//its not on the closed list.
				proxy_map[(int)start_y][(int)start_x] = start;
				openlist.insert(std::make_pair(h, std::make_pair(start.x, start.y)));
				std::pair<int, int> current_node;
				while (openlist.size() > 0)
				{
					iter++;
					n = *openlist.begin();//get the cheapest node
					//std::cout << n.second.first << ", " << n.second.second << std::endl;
					openlist.erase(openlist.begin());//pop the cheapest node
					current_node = { n.second.second,n.second.first };//keep hold of the current node. REMEMBER WQ ARE  X Y AND  MAP IS Y X
					proxy_map[current_node.first][current_node.second].open_list = 0;//reflect the change on the proxy map
					if (current_node.first == goal_y && current_node.second == goal_x)//reached the goal. REMEMBER WQ ARE  X Y AND  MAP IS Y X
					{
						n1 = n;
						points.clear();
						//std::cout << "FOUND!!!!" << iter  << std::endl;
						while (1)
						{
							//std::cout << "STUCK" << std::endl;
							//std::cout << n.second.first << ", " << n.second.second << std::endl;
							points.push_back({ n.second.first, n.second.second });
							float a = (float)n.second.first;
							float b = (float)n.second.second;
							Vector4 pp1 = { a,b, 0, 1 };
							if (n.second.second == -1 || n.second.first == -1)
							{
								break;
							}
							int x = proxy_map[n.second.second][n.second.first].parent_x;
							int y = proxy_map[n.second.second][n.second.first].parent_y;
							//std::cout << x << ", " << y << std::endl;
							if (x == -1 && y == -1)
							{
								if (points.size() >= index_offset)
								{
									end_x = points[points.size() - index_offset].x;
									end_y = points[points.size() - index_offset].y;
								}
								//if ((*MemoryManager::GetInstance()->ObjectPool)[3].pBd)
								{
									(*MemoryManager::GetInstance()->ObjectPool)[guide_key].pBd->mPos_x = end_x;
									(*MemoryManager::GetInstance()->ObjectPool)[guide_key].pBd->mPos_y = end_y;
									(*MemoryManager::GetInstance()->ObjectPool)[guide_key].pTr->mPositionX = end_x;
									(*MemoryManager::GetInstance()->ObjectPool)[guide_key].pTr->mPositionY = end_y;
									(*MemoryManager::GetInstance()->ObjectPool)[guide_key].pBd->mPos_x = (*MemoryManager::GetInstance()->ObjectPool)[guide_key].pTr->mPositionX;
									(*MemoryManager::GetInstance()->ObjectPool)[guide_key].pBd->mPos_y = (*MemoryManager::GetInstance()->ObjectPool)[guide_key].pTr->mPositionY;
									(*MemoryManager::GetInstance()->ObjectPool)[guide_key].pBd->mPos_z = (*MemoryManager::GetInstance()->ObjectPool)[guide_key].pTr->mPositionZ;
								}

								break;
							}
							n.second = { x , y };
							a = (float)n.second.first;
							b = (float)n.second.second;
							Vector4 pp2 = { a, b, 0, 1 };
							GraphicsManager::GetInstance()->AddDrawingLine(pp1, pp2, Vector3(1, 1, 0), false);
						}
						//std::cout << "ASDASD" << std::endl;
						break;
						//track back
					}
					//if not we analyze the neighbors, lets get the adjacent neighbors first, top, down, left, right
					//top
					if (current_node.first + 1 < floor_scale_y && map[current_node.first + 1][current_node.second].is_wall == false)
					{
						if (proxy_map[current_node.first + 1][current_node.second].open_list == 0 && proxy_map[current_node.first + 1][current_node.second].closed_list == 0)
							//if it is on none of the lists, add it to the openlist
						{
							node new_node;
							new_node.gx = proxy_map[current_node.first][current_node.second].gx + 1;//distance till there.
							h = Octile(current_node.second, current_node.first + 1, goal_x, goal_y);
							h = roundf(h * 100) / 100;
							new_node.hx = h;
							new_node.fx = new_node.gx + new_node.hx;//got the fx for the new node.
							new_node.y = current_node.first + 1;//assigning it its id in the graph.
							new_node.x = current_node.second;//assigning it its id in the graph.
							new_node.parent_x = current_node.second;//give it its parent id.
							new_node.parent_y = current_node.first;//give it its parent id.
							new_node.open_list = 1;
							new_node.closed_list = 0;
							proxy_map[current_node.first + 1][current_node.second] = new_node;
							openlist.insert(std::make_pair(new_node.fx, std::make_pair(current_node.second, current_node.first + 1)));//insert into the openlist
						}
						else if (proxy_map[current_node.first + 1][current_node.second].open_list == 1 || proxy_map[current_node.first + 1][current_node.second].closed_list == 1)//if its on  either of the list
						{
							float new_cost = 0;
							new_cost = proxy_map[current_node.first][current_node.second].gx + 1 + Octile(current_node.second, current_node.first + 1, goal_x, goal_y);
							if (new_cost < proxy_map[current_node.first + 1][current_node.second].fx)//if the new cost is lesser
							{
								if (proxy_map[current_node.first + 1][current_node.second].closed_list == 1)//if its on closed list
								{
									proxy_map[current_node.first + 1][current_node.second].closed_list = 0;//not on closed list anymore
								}
								if (proxy_map[current_node.first + 1][current_node.second].open_list == 1)//remove it from the openlist
								{
									std::set <std::pair<float, std::pair<int, int>>>::iterator it = openlist.begin();
									while (it != openlist.end())
									{
										//std::cout << openlist.size() << std::endl;
										if (it->second.second == current_node.first + 1 || it->second.first == current_node.second)//open  list has x,y we are going y+1,x
										{
											openlist.erase(it);
											break;
										}
										it++;
									}
								}
								proxy_map[current_node.first + 1][current_node.second].gx = proxy_map[current_node.first][current_node.second].gx + 1;
								h = Octile(current_node.second, current_node.first + 1, goal_x, goal_y);
								h = roundf(h * 100) / 100;
								proxy_map[current_node.first + 1][current_node.second].hx = h;
								proxy_map[current_node.first + 1][current_node.second].fx = proxy_map[current_node.first + 1][current_node.second].gx + proxy_map[current_node.first + 1][current_node.second].hx;//give it a new weight
								proxy_map[current_node.first + 1][current_node.second].parent_x = current_node.second;//give it its parent id.
								proxy_map[current_node.first + 1][current_node.second].parent_y = current_node.first;//give it its parent id.
								proxy_map[current_node.first + 1][current_node.second].open_list = 1;
								proxy_map[current_node.first + 1][current_node.second].closed_list = 0;
								openlist.insert(std::make_pair(proxy_map[current_node.first + 1][current_node.second].fx, std::make_pair(current_node.second, current_node.first + 1)));
							}
						}
					}
					//Top Right
					if (current_node.first + 1 < floor_scale_y && map[current_node.first + 1][current_node.second + 1].is_wall == false && current_node.second + 1 < floor_scale_x)
					{
						if (proxy_map[current_node.first + 1][current_node.second + 1].open_list == 0 && proxy_map[current_node.first + 1][current_node.second + 1].closed_list == 0)
							//if it is on none of the lists, add it to the openlist
						{
							node new_node;
							new_node.gx = proxy_map[current_node.first][current_node.second].gx + 1.414f;//distance till there.
							h = Octile(current_node.second + 1, current_node.first + 1, goal_x, goal_y);
							h = roundf(h * 100) / 100;
							new_node.hx = h;
							new_node.fx = new_node.gx + new_node.hx;//got the fx for the new node.
							new_node.y = current_node.first + 1;//assigning it its id in the graph.
							new_node.x = current_node.second + 1;//assigning it its id in the graph.
							new_node.parent_x = current_node.second;//give it its parent id.
							new_node.parent_y = current_node.first;//give it its parent id.
							new_node.open_list = 1;
							new_node.closed_list = 0;
							proxy_map[current_node.first + 1][current_node.second + 1] = new_node;
							openlist.insert(std::make_pair(new_node.fx, std::make_pair(current_node.second + 1, current_node.first + 1)));//insert into the openlist
						}
						else if (proxy_map[current_node.first + 1][current_node.second + 1].open_list == 1 || proxy_map[current_node.first + 1][current_node.second + 1].closed_list == 1)//if its on  either of the list
						{
							float new_cost = 0;
							new_cost = proxy_map[current_node.first][current_node.second].gx + 1.414f + Octile(current_node.second + 1, current_node.first + 1, goal_x, goal_y);
							if (new_cost < proxy_map[current_node.first + 1][current_node.second + 1].fx)//if the new cost is lesser
							{
								if (proxy_map[current_node.first + 1][current_node.second + 1].closed_list == 1)//if its on closed list
								{
									proxy_map[current_node.first + 1][current_node.second + 1].closed_list = 0;//not on closed list anymore
								}
								if (proxy_map[current_node.first + 1][current_node.second + 1].open_list == 1)//remove it from the openlist
								{
									std::set <std::pair<float, std::pair<int, int>>>::iterator it = openlist.begin();
									while (it != openlist.end())
									{
										//std::cout << openlist.size() << std::endl;
										if (it->second.second == current_node.first + 1 || it->second.first == current_node.second + 1)//open  list has x,y we are going y+1,x
										{
											openlist.erase(it);
											break;
										}
										it++;
									}
								}
								proxy_map[current_node.first + 1][current_node.second + 1].gx = proxy_map[current_node.first][current_node.second].gx + 1.414f;
								h = Octile(current_node.second + 1, current_node.first + 1, goal_x, goal_y);
								h = roundf(h * 100) / 100;
								proxy_map[current_node.first + 1][current_node.second + 1].hx = h;
								proxy_map[current_node.first + 1][current_node.second + 1].fx = proxy_map[current_node.first + 1][current_node.second + 1].gx + proxy_map[current_node.first + 1][current_node.second + 1].hx;//give it a new weight
								proxy_map[current_node.first + 1][current_node.second + 1].parent_x = current_node.second;//give it its parent id.
								proxy_map[current_node.first + 1][current_node.second + 1].parent_y = current_node.first;//give it its parent id.
								proxy_map[current_node.first + 1][current_node.second + 1].open_list = 1;
								proxy_map[current_node.first + 1][current_node.second + 1].closed_list = 0;
								openlist.insert(std::make_pair(proxy_map[current_node.first + 1][current_node.second + 1].fx, std::make_pair(current_node.second + 1, current_node.first + 1)));
							}
						}
					}
					//Top Left
					if (current_node.first + 1 < floor_scale_y && map[current_node.first + 1][current_node.second - 1].is_wall == false && current_node.second - 1 >= 0)
					{
						if (proxy_map[current_node.first + 1][current_node.second - 1].open_list == 0 && proxy_map[current_node.first + 1][current_node.second - 1].closed_list == 0)
							//if it is on none of the lists, add it to the openlist
						{
							node new_node;
							new_node.gx = proxy_map[current_node.first][current_node.second].gx + 1.414f;//distance till there.
							h = Octile(current_node.second - 1, current_node.first + 1, goal_x, goal_y);
							h = roundf(h * 100) / 100;
							new_node.hx = h;
							new_node.fx = new_node.gx + new_node.hx;//got the fx for the new node.
							new_node.y = current_node.first + 1;//assigning it its id in the graph.
							new_node.x = current_node.second - 1;//assigning it its id in the graph.
							new_node.parent_x = current_node.second;//give it its parent id.
							new_node.parent_y = current_node.first;//give it its parent id.
							new_node.open_list = 1;
							new_node.closed_list = 0;
							proxy_map[current_node.first + 1][current_node.second - 1] = new_node;
							openlist.insert(std::make_pair(new_node.fx, std::make_pair(current_node.second - 1, current_node.first + 1)));//insert into the openlist
						}
						else if (proxy_map[current_node.first + 1][current_node.second - 1].open_list == 1 || proxy_map[current_node.first + 1][current_node.second - 1].closed_list == 1)//if its on  either of the list
						{
							float new_cost = 0;
							new_cost = proxy_map[current_node.first][current_node.second].gx + 1.414f + Octile(current_node.second - 1, current_node.first + 1, goal_x, goal_y);
							if (new_cost < proxy_map[current_node.first + 1][current_node.second - 1].fx)//if the new cost is lesser
							{
								if (proxy_map[current_node.first + 1][current_node.second - 1].closed_list == 1)//if its on closed list
								{
									proxy_map[current_node.first + 1][current_node.second - 1].closed_list = 0;//not on closed list anymore
								}
								if (proxy_map[current_node.first + 1][current_node.second - 1].open_list == 1)//remove it from the openlist
								{
									std::set <std::pair<float, std::pair<int, int>>>::iterator it = openlist.begin();
									while (it != openlist.end())
									{
										//std::cout << openlist.size() << std::endl;
										if (it->second.second == current_node.first + 1 || it->second.first == current_node.second - 1)//open  list has x,y we are going y+1,x
										{
											openlist.erase(it);
											break;
										}
										it++;
									}
								}
								proxy_map[current_node.first + 1][current_node.second - 1].gx = proxy_map[current_node.first][current_node.second].gx + 1.414f;
								h = Octile(current_node.second - 1, current_node.first + 1, goal_x, goal_y);
								h = roundf(h * 100) / 100;
								proxy_map[current_node.first + 1][current_node.second - 1].hx = h;
								proxy_map[current_node.first + 1][current_node.second - 1].fx = proxy_map[current_node.first + 1][current_node.second - 1].gx + proxy_map[current_node.first + 1][current_node.second - 1].hx;//give it a new weight
								proxy_map[current_node.first + 1][current_node.second - 1].parent_x = current_node.second;//give it its parent id.
								proxy_map[current_node.first + 1][current_node.second - 1].parent_y = current_node.first;//give it its parent id.
								proxy_map[current_node.first + 1][current_node.second - 1].open_list = 1;
								proxy_map[current_node.first + 1][current_node.second - 1].closed_list = 0;
								openlist.insert(std::make_pair(proxy_map[current_node.first + 1][current_node.second - 1].fx, std::make_pair(current_node.second - 1, current_node.first + 1)));
							}
						}
					}
					//bot
					if (current_node.first - 1 >= 0 && map[current_node.first - 1][current_node.second].is_wall == false)
					{
						if (proxy_map[current_node.first - 1][current_node.second].open_list == 0 && proxy_map[current_node.first - 1][current_node.second].closed_list == 0)
							//if it is on none of the lists, add it to the openlist
						{
							node new_node;
							new_node.gx = proxy_map[current_node.first][current_node.second].gx + 1;//distance till there.
							h = Octile(current_node.second, current_node.first - 1, goal_x, goal_y);
							h = roundf(h * 100) / 100;
							new_node.hx = h;
							new_node.fx = new_node.gx + new_node.hx;//got the fx for the new node.
							new_node.y = current_node.first - 1;//assigning it its id in the graph.
							new_node.x = current_node.second;//assigning it its id in the graph.
							new_node.parent_x = current_node.second;//give it its parent id.
							new_node.parent_y = current_node.first;//give it its parent id.
							new_node.open_list = 1;
							new_node.closed_list = 0;
							proxy_map[current_node.first - 1][current_node.second] = new_node;
							openlist.insert(std::make_pair(new_node.fx, std::make_pair(current_node.second, current_node.first - 1)));//insert into the openlist
						}
						else if (proxy_map[current_node.first - 1][current_node.second].open_list == 1 || proxy_map[current_node.first - 1][current_node.second].closed_list == 1)//if its on  either of the list
						{
							float new_cost = 0;
							new_cost = proxy_map[current_node.first][current_node.second].gx + 1 + Octile(current_node.second, current_node.first - 1, goal_x, goal_y);
							if (new_cost < proxy_map[current_node.first - 1][current_node.second].fx)//if the new cost is lesser
							{
								if (proxy_map[current_node.first - 1][current_node.second].closed_list == 1)//if its on closed list
								{
									proxy_map[current_node.first - 1][current_node.second].closed_list = 0;//not on closed list anymore
								}
								if (proxy_map[current_node.first - 1][current_node.second].open_list == 1)//remove it from the openlist
								{
									std::set <std::pair<float, std::pair<int, int>>>::iterator it = openlist.begin();
									while (it != openlist.end())
									{
										//std::cout << openlist.size() << std::endl;
										if (it->second.second == current_node.first - 1 || it->second.first == current_node.second)//open  list has x,y we are going y+1,x
										{
											openlist.erase(it);
											break;
										}
										it++;
									}
								}
								proxy_map[current_node.first - 1][current_node.second].gx = proxy_map[current_node.first][current_node.second].gx + 1;
								h = Octile(current_node.second, current_node.first - 1, goal_x, goal_y);
								h = roundf(h * 100) / 100;
								proxy_map[current_node.first - 1][current_node.second].hx = h;
								proxy_map[current_node.first - 1][current_node.second].fx = proxy_map[current_node.first - 1][current_node.second].gx + proxy_map[current_node.first - 1][current_node.second].hx;//give it a new weight
								proxy_map[current_node.first - 1][current_node.second].parent_x = current_node.second;//give it its parent id.
								proxy_map[current_node.first - 1][current_node.second].parent_y = current_node.first;//give it its parent id.
								proxy_map[current_node.first - 1][current_node.second].open_list = 1;
								proxy_map[current_node.first - 1][current_node.second].closed_list = 0;
								openlist.insert(std::make_pair(proxy_map[current_node.first - 1][current_node.second].fx, std::make_pair(current_node.second, current_node.first - 1)));
							}
						}
					}
					//Bot Right
					if (current_node.first - 1 >= 0 && map[current_node.first - 1][current_node.second + 1].is_wall == false && current_node.second + 1 < floor_scale_x)
					{
						if (proxy_map[current_node.first - 1][current_node.second + 1].open_list == 0 && proxy_map[current_node.first - 1][current_node.second + 1].closed_list == 0)
							//if it is on none of the lists, add it to the openlist
						{
							node new_node;
							new_node.gx = proxy_map[current_node.first][current_node.second].gx + 1.414f;//distance till there.
							h = Octile(current_node.second + 1, current_node.first - 1, goal_x, goal_y);
							h = roundf(h * 100) / 100;
							new_node.hx = h;
							new_node.fx = new_node.gx + new_node.hx;//got the fx for the new node.
							new_node.y = current_node.first - 1;//assigning it its id in the graph.
							new_node.x = current_node.second + 1;//assigning it its id in the graph.
							new_node.parent_x = current_node.second;//give it its parent id.
							new_node.parent_y = current_node.first;//give it its parent id.
							new_node.open_list = 1;
							new_node.closed_list = 0;
							proxy_map[current_node.first - 1][current_node.second + 1] = new_node;
							openlist.insert(std::make_pair(new_node.fx, std::make_pair(current_node.second + 1, current_node.first - 1)));//insert into the openlist
						}
						else if (proxy_map[current_node.first - 1][current_node.second + 1].open_list == 1 || proxy_map[current_node.first - 1][current_node.second + 1].closed_list == 1)//if its on  either of the list
						{
							float new_cost = 0;
							new_cost = proxy_map[current_node.first][current_node.second].gx + 1.414f + Octile(current_node.second + 1, current_node.first - 1, goal_x, goal_y);
							if (new_cost < proxy_map[current_node.first - 1][current_node.second + 1].fx)//if the new cost is lesser
							{
								if (proxy_map[current_node.first - 1][current_node.second + 1].closed_list == 1)//if its on closed list
								{
									proxy_map[current_node.first - 1][current_node.second + 1].closed_list = 0;//not on closed list anymore
								}
								if (proxy_map[current_node.first - 1][current_node.second + 1].open_list == 1)//remove it from the openlist
								{
									std::set <std::pair<float, std::pair<int, int>>>::iterator it = openlist.begin();
									while (it != openlist.end())
									{
										//std::cout << openlist.size() << std::endl;
										if (it->second.second == (current_node.first - 1) || it->second.first == (current_node.second + 1))//open  list has x,y we are going y+1,x
										{
											openlist.erase(it);
											break;
										}
										it++;
									}
								}
								proxy_map[current_node.first - 1][current_node.second + 1].gx = proxy_map[current_node.first][current_node.second].gx + 1.414f;
								h = Octile(current_node.second + 1, current_node.first - 1, goal_x, goal_y);
								h = roundf(h * 100) / 100;
								proxy_map[current_node.first - 1][current_node.second + 1].hx = h;
								proxy_map[current_node.first - 1][current_node.second + 1].fx = proxy_map[current_node.first - 1][current_node.second + 1].gx + proxy_map[current_node.first - 1][current_node.second + 1].hx;//give it a new weight
								proxy_map[current_node.first - 1][current_node.second + 1].parent_x = current_node.second;//give it its parent id.
								proxy_map[current_node.first - 1][current_node.second + 1].parent_y = current_node.first;//give it its parent id.
								proxy_map[current_node.first - 1][current_node.second + 1].open_list = 1;
								proxy_map[current_node.first - 1][current_node.second + 1].closed_list = 0;
								openlist.insert(std::make_pair(proxy_map[current_node.first - 1][current_node.second + 1].fx, std::make_pair(current_node.second + 1, current_node.first - 1)));
							}
						}
					}
					//Bot Left
					if (current_node.first - 1 >= 0 && map[current_node.first - 1][current_node.second - 1].is_wall == false && current_node.second - 1 >= 0)
					{
						if (proxy_map[current_node.first - 1][current_node.second - 1].open_list == 0 && proxy_map[current_node.first - 1][current_node.second - 1].closed_list == 0)
							//if it is on none of the lists, add it to the openlist
						{
							node new_node;
							new_node.gx = proxy_map[current_node.first][current_node.second].gx + 1.414f;//distance till there.
							h = Octile(current_node.second - 1, current_node.first - 1, goal_x, goal_y);
							h = roundf(h * 100) / 100;
							new_node.hx = h;
							new_node.fx = new_node.gx + new_node.hx;//got the fx for the new node.
							new_node.y = current_node.first - 1;//assigning it its id in the graph.
							new_node.x = current_node.second - 1;//assigning it its id in the graph.
							new_node.parent_x = current_node.second;//give it its parent id.
							new_node.parent_y = current_node.first;//give it its parent id.
							new_node.open_list = 1;
							new_node.closed_list = 0;
							proxy_map[current_node.first - 1][current_node.second - 1] = new_node;
							openlist.insert(std::make_pair(new_node.fx, std::make_pair(current_node.second - 1, current_node.first - 1)));//insert into the openlist
						}
						else if (proxy_map[current_node.first - 1][current_node.second - 1].open_list == 1 || proxy_map[current_node.first - 1][current_node.second - 1].closed_list == 1)//if its on  either of the list
						{
							float new_cost = 0;
							new_cost = proxy_map[current_node.first][current_node.second].gx + 1.414f + Octile(current_node.second - 1, current_node.first - 1, goal_x, goal_y);
							if (new_cost < proxy_map[current_node.first - 1][current_node.second - 1].fx)//if the new cost is lesser
							{
								if (proxy_map[current_node.first - 1][current_node.second - 1].closed_list == 1)//if its on closed list
								{
									proxy_map[current_node.first - 1][current_node.second - 1].closed_list = 0;//not on closed list anymore
								}
								if (proxy_map[current_node.first - 1][current_node.second - 1].open_list == 1)//remove it from the openlist
								{
									std::set <std::pair<float, std::pair<int, int>>>::iterator it = openlist.begin();
									while (it != openlist.end())
									{
										//std::cout << openlist.size() << std::endl;
										if (it->second.second == current_node.first - 1 || it->second.first == current_node.second - 1)//open  list has x,y we are going y+1,x
										{
											openlist.erase(it);
											break;
										}
										it++;
									}
								}
								proxy_map[current_node.first - 1][current_node.second - 1].gx = proxy_map[current_node.first][current_node.second].gx + 1.414f;
								h = Octile(current_node.second - 1, current_node.first - 1, goal_x, goal_y);
								h = roundf(h * 100) / 100;
								proxy_map[current_node.first - 1][current_node.second - 1].hx = h;
								proxy_map[current_node.first - 1][current_node.second - 1].fx = proxy_map[current_node.first - 1][current_node.second - 1].gx + proxy_map[current_node.first - 1][current_node.second - 1].hx;//give it a new weight
								proxy_map[current_node.first - 1][current_node.second - 1].parent_x = current_node.second;//give it its parent id.
								proxy_map[current_node.first - 1][current_node.second - 1].parent_y = current_node.first;//give it its parent id.
								proxy_map[current_node.first - 1][current_node.second - 1].open_list = 1;
								proxy_map[current_node.first - 1][current_node.second - 1].closed_list = 0;
								openlist.insert(std::make_pair(proxy_map[current_node.first - 1][current_node.second - 1].fx, std::make_pair(current_node.second - 1, current_node.first - 1)));
							}
						}
					}
					//right
					if (current_node.second + 1 < floor_scale_x && map[current_node.first][current_node.second + 1].is_wall == false)
					{
						if (proxy_map[current_node.first][current_node.second + 1].open_list == 0 && proxy_map[current_node.first][current_node.second + 1].closed_list == 0)
							//if it is on none of the lists, add it to the openlist
						{
							node new_node;
							new_node.gx = proxy_map[current_node.first][current_node.second].gx + 1;//distance till there.
							h = Octile(current_node.second + 1, current_node.first, goal_x, goal_y);
							h = roundf(h * 100) / 100;
							new_node.hx = h;
							new_node.fx = new_node.gx + new_node.hx;//got the fx for the new node.
							new_node.y = current_node.first;//assigning it its id in the graph.
							new_node.x = current_node.second + 1;//assigning it its id in the graph.
							new_node.parent_x = current_node.second;//give it its parent id.
							new_node.parent_y = current_node.first;//give it its parent id.
							new_node.open_list = 1;
							new_node.closed_list = 0;
							proxy_map[current_node.first][current_node.second + 1] = new_node;
							openlist.insert(std::make_pair(new_node.fx, std::make_pair(current_node.second + 1, current_node.first)));//insert into the openlist
						}
						else if (proxy_map[current_node.first][current_node.second + 1].open_list == 1 || proxy_map[current_node.first][current_node.second + 1].closed_list == 1)//if its on  either of the list
						{
							float new_cost = 0;
							new_cost = proxy_map[current_node.first][current_node.second].gx + 1 + Octile(current_node.second + 1, current_node.first, goal_x, goal_y);
							if (new_cost < proxy_map[current_node.first][current_node.second + 1].fx)//if the new cost is lesser
							{
								if (proxy_map[current_node.first][current_node.second + 1].closed_list == 1)//if its on closed list
								{
									proxy_map[current_node.first][current_node.second + 1].closed_list = 0;//not on closed list anymore
								}
								if (proxy_map[current_node.first][current_node.second + 1].open_list == 1)//remove it from the openlist
								{
									std::set <std::pair<float, std::pair<int, int>>>::iterator it = openlist.begin();
									while (it != openlist.end())
									{
										//std::cout << openlist.size() << std::endl;
										if (it->second.second == current_node.first || it->second.first == current_node.second + 1)//open  list has x,y we are going y+1,x
										{
											openlist.erase(it);
											break;
										}
										it++;
									}
								}
								proxy_map[current_node.first][current_node.second + 1].gx = proxy_map[current_node.first][current_node.second].gx + 1;
								h = Octile(current_node.second + 1, current_node.first, goal_x, goal_y);
								h = roundf(h * 100) / 100;
								proxy_map[current_node.first][current_node.second + 1].hx = h;
								proxy_map[current_node.first][current_node.second + 1].fx = proxy_map[current_node.first][current_node.second + 1].gx + proxy_map[current_node.first][current_node.second + 1].hx;//give it a new weight
								proxy_map[current_node.first][current_node.second + 1].parent_x = current_node.second;//give it its parent id.
								proxy_map[current_node.first][current_node.second + 1].parent_y = current_node.first;//give it its parent id.
								proxy_map[current_node.first][current_node.second + 1].open_list = 1;
								proxy_map[current_node.first][current_node.second + 1].closed_list = 0;
								openlist.insert(std::make_pair(proxy_map[current_node.first][current_node.second + 1].fx, std::make_pair(current_node.second + 1, current_node.first)));
							}
						}
					}
					//left
					if (current_node.second - 1 >= 0 && map[current_node.first][current_node.second - 1].is_wall == false)
					{
						if (proxy_map[current_node.first][current_node.second - 1].open_list == 0 && proxy_map[current_node.first][current_node.second - 1].closed_list == 0)
							//if it is on none of the lists, add it to the openlist
						{
							node new_node;
							new_node.gx = proxy_map[current_node.first][current_node.second].gx + 1;//distance till there.
							h = Octile(current_node.second - 1, current_node.first, goal_x, goal_y);
							h = roundf(h * 100) / 100;
							new_node.hx = h;
							new_node.fx = new_node.gx + new_node.hx;//got the fx for the new node.
							new_node.y = current_node.first;//assigning it its id in the graph.
							new_node.x = current_node.second - 1;//assigning it its id in the graph.
							new_node.parent_x = current_node.second;//give it its parent id.
							new_node.parent_y = current_node.first;//give it its parent id.
							new_node.open_list = 1;
							new_node.closed_list = 0;
							proxy_map[current_node.first][current_node.second - 1] = new_node;
							openlist.insert(std::make_pair(new_node.fx, std::make_pair(current_node.second - 1, current_node.first)));//insert into the openlist
						}
						else if (proxy_map[current_node.first][current_node.second - 1].open_list == 1 || proxy_map[current_node.first][current_node.second - 1].closed_list == 1)//if its on  either of the list
						{
							float new_cost = 0;
							new_cost = proxy_map[current_node.first][current_node.second].gx + 1 + Octile(current_node.second - 1, current_node.first, goal_x, goal_y);
							if (new_cost < proxy_map[current_node.first][current_node.second - 1].fx)//if the new cost is lesser
							{
								if (proxy_map[current_node.first][current_node.second - 1].closed_list == 1)//if its on closed list
								{
									proxy_map[current_node.first][current_node.second - 1].closed_list = 0;//not on closed list anymore
								}
								if (proxy_map[current_node.first][current_node.second - 1].open_list == 1)//remove it from the openlist
								{
									std::set <std::pair<float, std::pair<int, int>>>::iterator it = openlist.begin();
									while (it != openlist.end())
									{
										//std::cout << openlist.size() << std::endl;
										if (it->second.second == current_node.first || it->second.first == current_node.second - 1)//open  list has x,y we are going y+1,x
										{
											openlist.erase(it);
											break;
										}
										it++;
									}
								}
								proxy_map[current_node.first][current_node.second - 1].gx = proxy_map[current_node.first][current_node.second].gx + 1;
								h = Octile(current_node.second - 1, current_node.first, goal_x, goal_y);
								h = roundf(h * 100) / 100;
								proxy_map[current_node.first][current_node.second - 1].hx = h;
								proxy_map[current_node.first][current_node.second - 1].fx = proxy_map[current_node.first][current_node.second + 1].gx + proxy_map[current_node.first][current_node.second + 1].hx;//give it a new weight
								proxy_map[current_node.first][current_node.second - 1].parent_x = current_node.second;//give it its parent id.
								proxy_map[current_node.first][current_node.second - 1].parent_y = current_node.first;//give it its parent id.
								proxy_map[current_node.first][current_node.second - 1].open_list = 1;
								proxy_map[current_node.first][current_node.second - 1].closed_list = 0;
								openlist.insert(std::make_pair(proxy_map[current_node.first][current_node.second - 1].fx, std::make_pair(current_node.second - 1, current_node.first)));
							}
						}
					}
					proxy_map[current_node.first][current_node.second].closed_list = 1;//put this on the closed list
				}
			}
			path_calc = true;
			n = n1;
			while (1)
			{
				// std::cout << n.second.first << ", " << n.second.second << std::endl;

				float a = (float)n.second.first;
				float b = (float)n.second.second;
				Vector4 pp1 = { a,b, 10, 1 };
				int x = proxy_map[n.second.second][n.second.first].parent_x;
				int y = proxy_map[n.second.second][n.second.first].parent_y;
				if (x == -1 || y == -1)
				{
					break;
				}
				n.second = { x , y };
				a = (float)n.second.first;
				b = (float)n.second.second;
				Vector4 pp2 = { a, b, 10, 1 };
				GraphicsManager::GetInstance()->AddDrawingLine(pp1, pp2, Vector3(0, 1, 0), false);
			}
			Body* b1 = (*MemoryManager::GetInstance()->ObjectPool)[guide_key].pBd;
			Body* b2 = this->pBd;
			if (!CollisionManager::GetInstance()->CheckCollisionCuboidCuboid(b1->mShape, b1->mPos_x, b1->mPos_y, b1->mPos_z, b2->mShape, b2->mPos_x, b2->mPos_y, b2->mPos_z))
			{
				index_offset++;
				int size_check = points.size() - index_offset;
				if (size_check > 0)
				{
					end_x = points[points.size() - index_offset].x;
					end_y = points[points.size() - index_offset].y;
					start_x = (*MemoryManager::GetInstance()->ObjectPool)[guide_key].pBd->mPos_x;
					start_y = (*MemoryManager::GetInstance()->ObjectPool)[guide_key].pBd->mPos_y;
					(*MemoryManager::GetInstance()->ObjectPool)[guide_key].pBd->mPos_x = end_x;
					(*MemoryManager::GetInstance()->ObjectPool)[guide_key].pBd->mPos_y = end_y;
					(*MemoryManager::GetInstance()->ObjectPool)[guide_key].pTr->mPositionX = end_x;
					(*MemoryManager::GetInstance()->ObjectPool)[guide_key].pTr->mPositionY = end_y;
				}
			}
			if (deltaTime < 0.05 && wentin == 1)
			{
				/*start_x = points[points.size() - 1].x;
				start_y = points[points.size() - 1].y;*/
				move_x = end_x - start_x;
				move_y = end_y - start_y;
				float newRot = atan2(move_y, move_x);
				//newRot = acos(newRot) ;
				//std::cout << end_x << ", " << end_y << ", " << newRot << std::endl;
				this->pBd->mVelo_x = 1 * cos(newRot);
				this->pBd->mVelo_y = 1 * sin(newRot);
			}
		}
		if (mAffectedBy)
		{
			EffectDamage* pED = new EffectDamage();
			pED->applyEffect(mAffectedBy, this, deltaTime);
			delete pED;
			mAffectedBy->reduceTime(deltaTime);
			if (mAffectedBy->getEffectDuration() <= 0)
			{
				delete mAffectedBy;
				mAffectedBy = nullptr;
			}
		}
	}
	else
	{
	if (!inc)
	{
		Player* pl = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
		//pl->score++;
	}
	inc = true;
	pBd->mPos_z = -500;
	}

}
float Charger::Octile(float start_x, float start_y, float goal_x, float goal_y)
{
	float x_diff = abs(start_x - goal_x);
	float y_diff = abs(start_y - goal_y);
	float min_x_y, max_x_y;
	if (x_diff <= y_diff)
	{
		min_x_y = x_diff;
		max_x_y = y_diff;
	}
	else
	{
		min_x_y = y_diff;
		max_x_y = x_diff;
	}
	return ((min_x_y * 1.414f) + abs(x_diff - y_diff));
}

void Charger::HandleEvent(Event* pEventToHandle)
{
	if (pEventToHandle)
	{
		if (pEventToHandle->mEventType == EVENT_TYPE::HIT_EVENT)
		{
			HitEvent* pHit = static_cast<HitEvent*>(pEventToHandle);
			pHit->PerformEnemyDamage(this);
			delete pEventToHandle;

		}
		if (pEventToHandle->mEventType == EVENT_TYPE::COLLIDE_EVENT)
		{
			float top, bottom, left, right, in, out;

			CollisionEvent* pEvent = static_cast<CollisionEvent*>(pEventToHandle);
			ENTITY_TYPE b1 = pEvent->mpBody1->mpEnowner->GetEntityType();
			ENTITY_TYPE b2 = pEvent->mpBody2->mpEnowner->GetEntityType();

			if (b1 == ENTITY_TYPE::CHARGER)
			{
				if (b2 == ENTITY_TYPE::RIGHT)
				{
					Charger* p = static_cast<Charger*>((*MemoryManager::GetInstance()->ObjectPool)[this->pBd->key].pEn);
					float player_x = p->pBd->mPos_x;
					float player_y = p->pBd->mPos_y;
					float result_x = player_x - (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_x;
					float result_y = player_y - (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_y;
					float newRot = atan2(result_y, result_x);
					p->pBd->mVelo_x = 10 * cos(newRot);
					p->pBd->mVelo_y = 10 * sin(newRot);
					p->got_hit_counter = 15;
					GraphicsManager::GetInstance()->SetCameraShack(10, 0.5);

					{
						Particle temp;
						temp.age = 0.7;
						temp.position = Vector4(p->pBd->mPos_x, p->pBd->mPos_y, p->pBd->mPos_z, 0.5);
						temp.TextureIndex = 1;
						temp.velocity = Vector4(3, 3, 3, 0.5);
						ParticleManager::GetInstance()->SphereEmit(40, temp);
					}
				}
				if (b2 == ENTITY_TYPE::LEFT)
				{
					Charger* p = static_cast<Charger*>((*MemoryManager::GetInstance()->ObjectPool)[this->pBd->key].pEn);
					float player_x = p->pBd->mPos_x;
					float player_y = p->pBd->mPos_y;
					float result_x = player_x - (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_x;
					float result_y = player_y - (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_y;
					float newRot = atan2(result_y, result_x);
					GraphicsManager::GetInstance()->SetCameraShack(10, 0.1);
					p->pBd->mVelo_x = 2 * cos(newRot);
					p->pBd->mVelo_y = 2 * sin(newRot);
					p->got_hit_counter = 2;
					{
						Particle temp;
						temp.age = 0.7;
						temp.position = Vector4(p->pBd->mPos_x, p->pBd->mPos_y, p->pBd->mPos_z, 0.5);
						temp.TextureIndex = 1;
						temp.velocity = Vector4(3, 3, 3, 0.5);
						ParticleManager::GetInstance()->SphereEmit(40, temp);
					}
				}
				if (b2 == ENTITY_TYPE::CHARGER)
				{
					top = abs((pEvent->mpBody2->mPos_z + pEvent->mpBody2->mHeight) - (pEvent->mpBody1->mPos_z - pEvent->mpBody1->mHeight));
					bottom = abs((pEvent->mpBody2->mPos_z - pEvent->mpBody2->mHeight) - (pEvent->mpBody1->mPos_z + pEvent->mpBody1->mHeight));
					left = abs((pEvent->mpBody2->mPos_x + pEvent->mpBody2->mWidth) - (pEvent->mpBody1->mPos_x - pEvent->mpBody1->mWidth));
					right = abs((pEvent->mpBody2->mPos_x - pEvent->mpBody2->mWidth) - (pEvent->mpBody1->mPos_x + pEvent->mpBody1->mWidth));
					in = abs((pEvent->mpBody2->mPos_y + pEvent->mpBody2->mDepth) - (pEvent->mpBody1->mPos_y - pEvent->mpBody1->mDepth));
					out = abs((pEvent->mpBody2->mPos_y - pEvent->mpBody2->mDepth) - (pEvent->mpBody1->mPos_y + pEvent->mpBody1->mDepth));
					if (top < bottom && top < left && top < right && top < in && top < out)//condition for top
					{
						pEvent->mpBody1->body_State = ON_SURFACE;
						pEvent->mpBody1->mPos_z = pEvent->mpBody2->mPos_z + pEvent->mpBody2->mHeight + pEvent->mpBody1->mHeight;
						pEvent->mpBody1->mVelo_z = 0;
					}
					else if (bottom < top && bottom < left && bottom < right && bottom < in && bottom < out)//condition for bottom
					{
						pEvent->mpBody2->mVelo_z = 0;
						pEvent->mpBody2->body_State = ON_SURFACE;
						pEvent->mpBody2->mPos_z = pEvent->mpBody1->mPos_z + pEvent->mpBody1->mHeight + pEvent->mpBody2->mHeight;
					}
					else if (in < right && in < left && in < top && in < bottom && in < out)
					{
						pEvent->mpBody2->mPos_y = pEvent->mpBody1->mPos_y - pEvent->mpBody1->mDepth - pEvent->mpBody2->mDepth;
						float t = pEvent->mpBody2->mVelo_y;
						pEvent->mpBody2->mVelo_y = pEvent->mpBody1->mVelo_y;
						pEvent->mpBody1->mVelo_y = t;
					}
					else if (out < right && out < left && out < top && out < bottom && out < in)
					{
						pEvent->mpBody1->mPos_y = pEvent->mpBody2->mPos_y - pEvent->mpBody2->mDepth - pEvent->mpBody1->mDepth;
						float t = pEvent->mpBody2->mVelo_y;
						pEvent->mpBody2->mVelo_y = pEvent->mpBody1->mVelo_y;
						pEvent->mpBody1->mVelo_y = t;
					}
					else if (right < in && right < left && right < top && right < bottom && right < out)
					{
						pEvent->mpBody1->mPos_x = pEvent->mpBody2->mPos_x - pEvent->mpBody2->mWidth - pEvent->mpBody1->mWidth;
						float t = pEvent->mpBody2->mVelo_x;
						pEvent->mpBody2->mVelo_x = pEvent->mpBody1->mVelo_x;
						pEvent->mpBody1->mVelo_x = t;
					}
					else//condition for left
					{
						pEvent->mpBody2->mPos_x = pEvent->mpBody1->mPos_x - pEvent->mpBody1->mWidth - pEvent->mpBody2->mWidth;
						float t = pEvent->mpBody2->mVelo_x;
						pEvent->mpBody2->mVelo_x = pEvent->mpBody1->mVelo_x;
						pEvent->mpBody1->mVelo_x = t;
					}

				}

				else if (b2 == ENTITY_TYPE::PLAYER)
				{
					left = abs((pEvent->mpBody2->mPos_x + pEvent->mpBody2->mWidth) - (pEvent->mpBody1->mPos_x - pEvent->mpBody1->mWidth));
					right = abs((pEvent->mpBody2->mPos_x - pEvent->mpBody2->mWidth) - (pEvent->mpBody1->mPos_x + pEvent->mpBody1->mWidth));
					in = abs((pEvent->mpBody2->mPos_y + pEvent->mpBody2->mDepth) - (pEvent->mpBody1->mPos_y - pEvent->mpBody1->mDepth));
					out = abs((pEvent->mpBody2->mPos_y - pEvent->mpBody2->mDepth) - (pEvent->mpBody1->mPos_y + pEvent->mpBody1->mDepth));

					(*MemoryManager::GetInstance()->ObjectPool)[2].pCr->usable = false;

					Player* player = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);

					HitEvent* newHit = new HitEvent();
					EventManager::GetInstance()->BroadcastToObject(newHit, player->key);
					delete newHit;


					if (right < left && right < in && right < out)//condition for right
					{
						pEvent->mpBody2->mVelo_x = 10;
						//c->mpBodies[0]->mVelo_x = -20;
					}
					else if (in < right && in < left && in < out)
					{
						pEvent->mpBody2->mVelo_y = -10;
						//c->mpBodies[0]->mVelo_y = 20;
					}
					else if (out < right && out < left && out < in)
					{
						pEvent->mpBody2->mVelo_y = 10;
						//c->mpBodies[0]->mVelo_y = -20;
					}
					else//condition for left
					{
						pEvent->mpBody2->mVelo_x = -10;
						//c->mpBodies[0]->mVelo_x = 20;
					}
				}
			}
			else if (b2 == ENTITY_TYPE::CHARGER)
			{
				if (b1 == ENTITY_TYPE::RIGHT)
				{
					Charger* p = static_cast<Charger*>((*MemoryManager::GetInstance()->ObjectPool)[this->pBd->key].pEn);
					float player_x = p->pBd->mPos_x;
					float player_y = p->pBd->mPos_y;
					float result_x = player_x - (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_x;
					float result_y = player_y - (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_y;
					float newRot = atan2(result_y, result_x);
					p->pBd->mVelo_x = 10 * cos(newRot);
					p->pBd->mVelo_y = 10 * sin(newRot);
					p->got_hit_counter = 15;
					GraphicsManager::GetInstance()->SetCameraShack(10, 0.5);
					{
						Particle temp;
						temp.age = 0.7;
						temp.position = Vector4(p->pBd->mPos_x, p->pBd->mPos_y, p->pBd->mPos_z, 0.5);
						temp.TextureIndex = 1;
						temp.velocity = Vector4(3, 3, 3, 0.5);
						ParticleManager::GetInstance()->SphereEmit(40, temp);
					}
				}
				if (b1 == ENTITY_TYPE::LEFT)
				{
					Charger* p = static_cast<Charger*>((*MemoryManager::GetInstance()->ObjectPool)[this->pBd->key].pEn);
					float player_x = p->pBd->mPos_x;
					float player_y = p->pBd->mPos_y;
					float result_x = player_x - (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_x;
					float result_y = player_y - (*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mPos_y;
					float newRot = atan2(result_y, result_x);
					GraphicsManager::GetInstance()->SetCameraShack(10, 0.1);
					p->pBd->mVelo_x = 2 * cos(newRot);
					p->pBd->mVelo_y = 2 * sin(newRot);
					p->got_hit_counter = 2;
					{
						Particle temp;
						temp.age = 0.7;
						temp.position = Vector4(p->pBd->mPos_x, p->pBd->mPos_y, p->pBd->mPos_z, 0.5);
						temp.TextureIndex = 1;
						temp.velocity = Vector4(3, 3, 3, 0.5);
						ParticleManager::GetInstance()->SphereEmit(40, temp);
					}
				}
				if (b1 == ENTITY_TYPE::PLAYER)
				{
					left = abs((pEvent->mpBody1->mPos_x + pEvent->mpBody1->mWidth) - (pEvent->mpBody2->mPos_x - pEvent->mpBody2->mWidth));
					right = abs((pEvent->mpBody1->mPos_x - pEvent->mpBody1->mWidth) - (pEvent->mpBody2->mPos_x + pEvent->mpBody2->mWidth));
					in = abs((pEvent->mpBody1->mPos_y + pEvent->mpBody1->mDepth) - (pEvent->mpBody2->mPos_y - pEvent->mpBody2->mDepth));
					out = abs((pEvent->mpBody1->mPos_y - pEvent->mpBody1->mDepth) - (pEvent->mpBody2->mPos_y + pEvent->mpBody2->mDepth));


					(*MemoryManager::GetInstance()->ObjectPool)[2].pCr->usable = false;

					Player* player = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);

					HitEvent* newHit = new HitEvent();
					EventManager::GetInstance()->BroadcastToObject(newHit, player->key);
					delete newHit;

					if (right < left && right < in && right < out)//condition for right
					{
						pEvent->mpBody1->mVelo_x = 10;
						//c->mpBodies[1]->mVelo_x = -20;
					}
					else if (in < right && in < left && in < out)
					{
						pEvent->mpBody1->mVelo_y = -10;
						//c->mpBodies[1]->mVelo_y = 20;
					}
					else if (out < right && out < left && out < in)
					{
						pEvent->mpBody1->mVelo_y = 10;
						//c->mpBodies[1]->mVelo_y = -20;
					}
					else//condition for left
					{
						pEvent->mpBody1->mVelo_x = -10;
						//c->mpBodies[1]->mVelo_x = 20;
					}
				}
			}
		}
	}
}