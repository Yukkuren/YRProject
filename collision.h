#pragma once

#include "framework.h"
#include "sprite.h"
#include <memory>

class CollisionSprite
{
public:
	std::unique_ptr<Sprite> box = nullptr;
	std::unique_ptr<Sprite> circle = nullptr;

	static CollisionSprite& getInstance()
	{
		static CollisionSprite instance;
		return instance;
	}

	void SetSprite()
	{
		if (box == nullptr)
		{
			box = std::make_unique<Sprite>(L"./Data/Shader/collision.png");
			box->LoadGraph(100.0f, 100.0f);
		}
		if (circle == nullptr)
		{
			circle = std::make_unique<Sprite>(L"./Data/Shader/collision_circle.png");
			circle->LoadGraph(100.0f, 100.0f);
		}
		
	}
};

#define pColSprite		(CollisionSprite::getInstance())

class Collision
{
private:
	DirectX::XMFLOAT2	boxsize;
	FLOAT				circle_size;
public:
	/*std::unique_ptr<Sprite> box;
	std::unique_ptr<Sprite> circle;*/

	void SetBox(DirectX::XMFLOAT2 boxsize)
	{
		this->boxsize = boxsize;
		//pColSprite.box = std::make_unique<Sprite>(device, L"./Data/Image/collision.png");
		pColSprite.box->LoadGraph(100.0f, 100.0f);
	}

	void SetCircle(FLOAT circle_size)
	{
		this->circle_size = circle_size;
		//circle = std::make_unique<Sprite>(device, L"./Data/Image/collision_circle.png");
		pColSprite.circle->LoadGraph(100.0f, 100.0f);
	}

	void SetSize(DirectX::XMFLOAT2 &boxsize)
	{
		this->boxsize = boxsize;
	}
	void SetSize(FLOAT &circle_size)
	{
		this->circle_size = circle_size;
	}

	DirectX::XMFLOAT2 &GetBoxSize()
	{
		return boxsize;
	}

	FLOAT &GetCircleSize()
	{
		return circle_size;
	}
	/*static Collision& getInstance()
	{
		static Collision instance;
		return instance;
	}*/

	void DrawBox(YRShader *shader,float posX, float posY, float angle = 0.0f, DirectX::XMFLOAT4 color = { 1,1,1,0.5f })
	{
		//FRAMEWORK.BlendSet(Blend::BLEND_MODE::ALPHA);
		pColSprite.box->DrawRotaGraph(shader,posX, posY, angle, boxsize, color);
		//FRAMEWORK.BlendSet(Blend::BLEND_MODE::NONE);
	}
	void DrawCircle(YRShader *shader,float posX, float posY, float angle = 0.0f, DirectX::XMFLOAT4 color = { 1,1,1,0.5f })
	{
		//FRAMEWORK.BlendSet(Blend::BLEND_MODE::ALPHA);
		pColSprite.circle->DrawCircleGraph(shader,posX, posY, angle, circle_size, color);
		//FRAMEWORK.BlendSet(Blend::BLEND_MODE::NONE);
	}
};

//#define COLLISION		(Collision::getInstance())
