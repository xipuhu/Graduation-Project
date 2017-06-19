#ifndef AUDIOS_H
#define AUDIOS_H

#include <GL/glew.h>
#include<IrrKlang\irrKlang.h>

using namespace irrklang;

class Audio
{
public:

	//音频
	ISoundEngine* SoundEngine;
	irrklang::ISound* bgMusic;
	const char* soundPath;
	
	Audio();
	void loadAudio(const char* path, bool isMusic){
		
		//当音频作为背景音乐来使用时
		if (isMusic==true)    
		  bgMusic = SoundEngine->play2D(path, true, true);
		else
		{
			bgMusic = NULL;
		}
		soundPath = path;
	}

	void play(){
			SoundEngine->play2D(soundPath, GL_FALSE);
	}

	void startPlay(){
		if (bgMusic){
			bgMusic->setIsPaused(false);	
		}
		  
	}
	void pausePlay(){
		if (bgMusic)
			bgMusic->setIsPaused(true);
	}

	void deleteSound(){
		if (bgMusic){
			bgMusic->drop();
			bgMusic = 0;
		}
	}
	void setVolume(float vloume){
		if (bgMusic)
			bgMusic->setVolume(vloume);
	}

	~Audio();

private:

};

Audio::Audio()
{
	SoundEngine = irrklang::createIrrKlangDevice();
}

Audio::~Audio()
{
	SoundEngine->drop();

}

#endif