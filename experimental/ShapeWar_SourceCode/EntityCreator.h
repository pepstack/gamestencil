//---------------------------------------------------------------------------------
// declaration	: Copyright (c), by XJ , 2014 . All right reserved.
// brief		: This file will define the Entity Creator that will supply some creator
//                method to create the entity in this game .
// author		: XJ
// date			: 2014 / 6 / 9
// version		: 1.0
//----------------------------------------------------------------------------------
#pragma once

namespace ShapeWar
{
	class EntityCreator
	{
	public:
		EntityCreator();
		~EntityCreator();

	public:
		/**
		* Create the Red cube in this game
		*/
		static _int64 createRedCube(float vx, float vy);

		/**
		* Create the Purple cube in this game
		*/
		static _int64 createPurpleCube(float vx, float vy);

		/**
		* Create the Orange cube in this game
		*/
		static _int64 createOrangeCube(float vx, float vy);

		/**
		* Create the Green cube in this game
		*/
		static _int64 createGreenCube(float vx, float vy);

		/**
		* Create the Sphere ball in this game
		*/
		static _int64 createSphereBall(float x, float y);  

		/**
		* Create the Player in this game
		*/
		static _int64 createPlayer();
	};
};