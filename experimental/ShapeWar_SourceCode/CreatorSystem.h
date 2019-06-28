//-------------------------------------------------------------------
// declaration	: Copyright (c), by XJ , 2014 . All right reserved .
// brief		: This file will define the Creator that will create
//                the cube in this game.
// author		: XJ
// date			: 2014 / 6 / 10
// version		: 1.0
//-------------------------------------------------------------------
#pragma once
#include"SystemManager.h"

namespace ShapeWar
{
	class CreatorSystem:public System
	{
	public:
		CreatorSystem(int _priority);
		~CreatorSystem();

	public:
		void enter();
		void excute(float dt);
		void exit();

	private:
		unsigned _int64 frames ;
	};
};