#include"AudioSystem.h"
#include<assert.h>
#include<Windows.h>

AudioSystem::AudioSystem()
{
	_clear();
}

AudioSystem::~AudioSystem()
{
	_clear();
}

AudioSystem* AudioSystem::sharedAudioSystem()
{
	static AudioSystem pAudio ;
	return &pAudio ;
}

bool AudioSystem::loadSound(std::string filePath, bool bLoop)
{
	assert(m_pFMODSystem != NULL && "You must call AudioSystem::init before");

	//Alloc the AudioObject
	AudioObject* _pObject = new AudioObject ;
	memset(_pObject,0,sizeof(AudioObject));

	//Create the specific sound
	FMOD_RESULT _result ;
	if(bLoop)
	{
		 _result = m_pFMODSystem->createSound(filePath.c_str(),FMOD_HARDWARE |FMOD_LOOP_NORMAL|FMOD_2D,
			0,&_pObject->pSound);
	}
	else
	{
		 _result = m_pFMODSystem->createSound(filePath.c_str(),FMOD_HARDWARE |FMOD_2D,
			0,&_pObject->pSound);
	}

	if(_result != FMOD_OK)
	{
		OutputDebugStringA(FMOD_ErrorString(_result));
		return false ;
	}

	//Find the sound file name
	int _nPos = filePath.find_last_of('//',filePath.length()-1);
	_pObject->fileName = new string(filePath.substr(_nPos+1));


	//Set the state
	_pObject->bIsPlaying = false ;

	//Add to the array
	m_AudioObjectArray.push_back(_pObject);

	return true ;
}

bool AudioSystem::playSound(std::string fileName)
{
	AudioObject * _pObject = _findObject(fileName);

	if(_pObject != NULL)
	{
		_pObject->bIsPlaying = true ;
		_pObject->bPaused = false ;
		m_pFMODSystem->playSound(_pObject->pSound,NULL,true,&_pObject->pChannel); //play sound to get the channel (the sound is paused now)
		_pObject->pChannel->setVolume(m_fVolume);								  //use the channel to set the volume
		_pObject->pChannel->setPaused(false);									  //start playing the sound
		return true ;
	}
	else
		return false ;

	/*if(_pObject != NULL&& _pObject->bIsPlaying == false)						  //sound must be Playing state
	{
		_pObject->bIsPlaying = true ;
		_pObject->bPaused = false ;
		m_pFMODSystem->playSound(_pObject->pSound,NULL,true,&_pObject->pChannel); //play sound to get the channel (the sound is paused now)
		_pObject->pChannel->setVolume(m_fVolume);								  //use the channel to set the volume
		_pObject->pChannel->setPaused(false);									  //start playing the sound
		return true ;
	}
	else
	{
		return false ;
	}*/
}

bool AudioSystem::stopSound(std::string fileName)
{
	AudioObject * _pObject = _findObject(fileName);
	if(_pObject != NULL&& _pObject->bIsPlaying == true)
	{
		_pObject->bIsPlaying = false ;
		_pObject->pChannel->stop();
		return true ;
	}

	return false ;
}

bool AudioSystem::pauseSound(std::string fileName, bool bPaused)
{
	AudioObject* _pObject = _findObject(fileName);

	return _pauseSoundWithObject(_pObject,bPaused);
}

bool AudioSystem::pauseAllSound(bool bPaused)
{
	bool _bRet = true ;
	vector<AudioObject*>::iterator it = m_AudioObjectArray.begin();
	for(;it != m_AudioObjectArray.end() ; )
	{
		AudioObject* _pObject = *it ;
		if(!_pauseSoundWithObject(_pObject,bPaused)) //if there is one failed , this method will return false
			_bRet = false ;
		it ++ ;
	}

	return _bRet ;
}

bool AudioSystem::_pauseSoundWithObject(AudioObject* pObject, bool bPaused)
{
	AudioObject* _pObject = pObject ;

	if(_pObject != NULL && _pObject->bIsPlaying == true)
	{
		_pObject->bPaused = bPaused ;
		_pObject->pChannel->setPaused(bPaused);

		return true ;
	}
	else
		return false ;
}

void AudioSystem::setVolume(float fVolume)
{
	if(fVolume < 0.0f)
		fVolume = 0.0f ;
	if(fVolume > 1.0f)
		fVolume = 1.0f ;
	m_fVolume = fVolume ;

	//Poll all the sound 
	vector<AudioObject*>::iterator it = m_AudioObjectArray.begin();
	for(;it != m_AudioObjectArray.end(); )
	{
		AudioObject* _pObject = * it ;
		bool _bPlaying = false ;
		_pObject->pChannel->isPlaying(&_bPlaying);
		if(_bPlaying)
		{
			_pObject->pChannel->setPaused(true);
			_pObject->pChannel->setVolume(m_fVolume);
			_pObject->pChannel->setPaused(false);
		}

		it ++ ;
	}
}

bool AudioSystem::init()
{
	//Create the fmod system
	FMOD_RESULT _result = FMOD::System_Create(&m_pFMODSystem);

	//Initialize the fmod system
	m_pFMODSystem->init(6,FMOD_INIT_NORMAL,NULL);

	//Set the volume
	m_fVolume = 0.8f ;

	m_AudioObjectArray.clear();

	return true ;
}

void AudioSystem::update()
{
	//Update the FMOD
	m_pFMODSystem->update();

	//Poll all the sound state
	vector<AudioObject*>::iterator it = m_AudioObjectArray.begin();
	for(;it != m_AudioObjectArray.end(); )
	{
		AudioObject* _pObject = * it ;
		bool _bPlaying = false ;
		bool _bPaused = false ;

		_pObject->pChannel->isPlaying(&_bPlaying);
		_pObject->pChannel->getPaused(&_bPaused);

		_pObject->bIsPlaying = _bPlaying ;
		_pObject->bPaused = _bPaused ;

		it ++ ;
	}
}


void AudioSystem::release()
{
	//Release the AudioObject array
	vector<AudioObject*>::iterator it = m_AudioObjectArray.begin();
	for(;it != m_AudioObjectArray.end(); )
	{
		AudioObject* _pObject = * it ;
		_pObject->pSound->release();
		//delete _pObject->pSound ;
		//delete _pObject->pChannel ;
		_pObject->fileName->clear();
		delete _pObject->fileName ;
		delete _pObject ; 
		_pObject = NULL ;
		it ++ ;
	}

	m_AudioObjectArray.clear();

	//Release the FMOD
	m_pFMODSystem->release();
}

void AudioSystem::remove(std::string fileName)
{
	//Remove the specific sound
	vector<AudioObject*>::iterator it = m_AudioObjectArray.begin();
	for(;it != m_AudioObjectArray.end(); )
	{
		AudioObject* _pObject = * it ;
		if(!fileName.compare(*_pObject->fileName))
		{
			_pObject->pSound->release();
			_pObject->fileName->clear();
			delete _pObject ;
			_pObject = NULL ;
			vector<AudioObject*>::iterator temp = it ;
			it ++ ;
			m_AudioObjectArray.erase(temp);
			break ;
		}
		else
		{
			it++;
		}
	}
}

void AudioSystem::_clear()
{
	m_AudioObjectArray.clear();
	m_pFMODSystem = NULL ;
	m_fVolume = 0.8f ;
}

AudioObject* AudioSystem::_findObject(std::string fileName)
{
	//Remove the specific sound
	vector<AudioObject*>::iterator it = m_AudioObjectArray.begin();
	for(;it != m_AudioObjectArray.end(); )
	{
		AudioObject* _pObject = * it ;
		if(!fileName.compare(*_pObject->fileName))
		{
			return _pObject ;
		}
		else
		{
			it++;
		}
	}

	return NULL ;
}