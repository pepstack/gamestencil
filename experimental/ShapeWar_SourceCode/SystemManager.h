//---------------------------------------------------------------------
// declaration	: Copyright (c), by XJ , 2014 . All right reserved .
// brief		: This file will define the System of the Entity-Component-System
//                and also define a system manager to manage all systems.
// author		: XJ
// date			: 2014 / 6 / 9
// version		: 1.0
//---------------------------------------------------------------------
#pragma once
#include<vector>
using namespace std ;

namespace ShapeWar
{
	/**
	* Define the base system class. All system will inherit from this base class.
	*/
	class System
	{
	public:
		System(int _priority);
		virtual ~System();

	public:
		virtual void enter() = 0 ;
		virtual void excute(float dt) = 0;
		virtual void exit() = 0 ;

	public:
		int priority ;
	};

	/**
	* Define the system manager
	*/
	class SystemManager
	{
	private:
		SystemManager();
		~SystemManager();

		/** Singleton getter*/
	public:
		static SystemManager* getSystemManager() ;

		/** Core method*/
	public:
		/**
		* Add one system to the system list
		*/
		void addSystem(System * system);

		/**
		* Update all the system
		*/
		void update(float dt);

		/**
		* Pause all the system
		*/
		void pause();

		/**
		* Resume all the system
		*/
		void resume();

	private:
		/**
		* Destroy all the systems
		*/
		void _destroy();

	private:
		std::vector<System*> system_list ;
		bool bPaused ;
	};
};