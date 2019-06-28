//--------------------------------------------------------------------
// declaration	: Copyright (c), by XJ , 2014 . All right reserved .
// brief		: This file will define the Render System.
// author		: XJ
// date			: 2014 / 6 / 9
// version		: 1.0
//--------------------------------------------------------------------
#pragma once
#include"SystemManager.h"
#include<cocos2d.h>
using namespace cocos2d ;

namespace ShapeWar
{
	class RenderSystem:public System
	{
	public:
		RenderSystem(int priority, CCNode* scene);
		~RenderSystem();

	public:
		void enter();
		void excute(float dt);
		void exit();

	private:
		cocos2d::CCNode* scene ;
	};
};