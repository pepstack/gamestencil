//-------------------------------------------------------------------------------
// declaration	: Copyright (c), by XJ , 2014 .All right reserved .
// brief		: This file will define the Collision Detection System.
// author		: XJ
// date			: 2014 / 6 / 10
// version		: 1.0
//-------------------------------------------------------------------------------
#pragma once
#include"SystemManager.h"

namespace ShapeWar
{
	class CollidDetectionSystem:public System
	{
	public:
		CollidDetectionSystem(int _priority);
		~CollidDetectionSystem();

	public:
		void enter();
		void excute(float dt);
		void exit();
	};
};