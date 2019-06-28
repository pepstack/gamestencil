//-----------------------------------------------------------------------------------
//declaration	:Copyright (c), by XJ . All right reserved .
//brief			: This AudioSystem use the FMOD Engine to play the sound in Game .
//file			: AudioSystem.h
//author		: XJ
//version		: 0.0
//date			: 2013 / 11 / 17
//------------------------------------------------------------------------------------
#pragma once

//include the header file
#include"fmod.hpp"
#include"fmod_errors.h"
#include<string>
#include<vector>
#pragma comment(lib,"fmod_vc.lib")
using namespace std ;


class AudioObject
{
public:
		std::string*	fileName   ;
		FMOD::Sound *	pSound     ;
		FMOD::Channel*	pChannel   ;
		bool            bPaused	   ;
		bool			bIsPlaying ;
};

//@note :
//The sound has two main state IsPlaying or not .
//When use the playSound() the AduioObject.bIsPlaying set true 
//and when use the stopSound() the AudioObject.bIsPlaying will set false .
//Apart from that, the AudioObject.bPaused is the chid state of IsPlaying .
//This means only if the AudioObject.bIsPlay is true ,then the AudioObject.bPaused does work .
//When the AudioObject.bIsPlaying is true, we can use the pauseSound() or pauseAllSound() to 
//pause or restart the specific sound .
//But when the AudioObject.bIsPlaying is false, we can not control the pause or non-pause state of the 
//object, because it hasn't been played .

//define the class
class AudioSystem
{
	//@name	: Contructor and descontructor
private:
	AudioSystem();
	~AudioSystem();

	//@name	: Singleton Getter
public:
	static AudioSystem* sharedAudioSystem();

	//@name	: Sound control method
public:
	//@breif	: Load the specificed sound file
	//@param	: filePath -- Sound file path
	//			  bLoop    -- if set true then the sound will play looped , otherwise not
	//@return	: if success return true otherwise return false
	bool loadSound(std::string filePath,bool bLoop);

	//@brief	: Play the specificed sound file .
	//@param	: fileName -- Sound file name
	//@return	: if success return true otherwise return false
	bool playSound(std::string fileName);

	//@brief	: Stop the specificed sound
	//@param	: Sound file name
	//@return	: if success return true otherwise return false
	bool stopSound(std::string);

	//@brief	: Pause the specificed sound
	//@param	: fileName -- Sound file name 
	//			  bPause   -- if set true then pause the sound , otherwise restart the sound
	//@return	: if success return true otherwise return false
	bool pauseSound(std::string,bool bPaused);
	
	//@brief	: Pause all the playing sound
	//@param	: See pauseSound
	bool pauseAllSound(bool bPaused);

	//@brief	: Set the volume of the AudioSystem
	//@param	: The volume of the all the sound. It's in the range[0,1]
	//			  0 mean no sound and 1 mean full sound .
	void setVolume(float) ;

	//@name	: AudioSystem core method
public:
	//@brief	: Initialize the AudioSystem
	//@return	: if success return true otherwise return false
	bool init();

	//@brief	: Update the AudioSystem
	void update();

	//@brief	: Release the AudioSystem's resource
	void release();

	//@brief	: Remove the specific AudioObject from the array
	void remove(std::string);
	
	//@name	: Internal useage method
private:
	//@brief	: Clear all the member property
	void _clear();

	//@brief	: Find the AudioObject
	//@param	: The sound's filename, just name not the aboslute path
	AudioObject* _findObject(std::string);

	bool _pauseSoundWithObject(AudioObject*, bool);

	//@name	: Internal member property
private:
	std::vector<AudioObject*>  m_AudioObjectArray ;
	float					   m_fVolume          ;
	FMOD::System*			   m_pFMODSystem	  ;
};
