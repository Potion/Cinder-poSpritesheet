//
//  Spritesheet.h
//  
//
//  Created by bruce on 8/26/14.
//
//

#pragma once

#include "cinder/gl/Texture.h"
#include "cinder/Json.h"
#include "cinder/Rect.h"

class Spritesheet;
typedef std::shared_ptr<Spritesheet> SpritesheetRef;

class Spritesheet {
public:
	static SpritesheetRef create(ci::gl::TextureRef texture, ci::JsonTree json);
	~Spritesheet();
	
	virtual void update();
	virtual void draw();
	void play() { mIsPlaying = true; }
	void pause() { mIsPlaying = false; }
	void setIsLoopingEnabled(bool isLooping) { mIsLooping = isLooping; }
	void setFrameRate(float frameRate) { mFPS = frameRate; }
	
protected:
	Spritesheet();
	
	void setup(ci::gl::TextureRef texture, ci::JsonTree json);
	
private:
	
	// Sprite metadata
	struct MetaData {
		MetaData(){};
		
		std::string image;
		ci::Vec2f size;
		float scale;
	};
	
	// Frame data structure
	struct FrameData {
		FrameData(){};
		
		std::string filename;
		ci::Area frame;
		bool rotated;
		bool trimmed;
		ci::Rectf spriteSourceSize;
		ci::Vec2f sourceSize;
	};
	
	ci::gl::TextureRef mTexture;
	MetaData mMetadata;
	std::vector<FrameData> mFrames;
	
	int mCurrentFrame;
	int mNumFrames;
	bool mIsPlaying;
	bool mIsLooping;
	float mFrameRate;
	float mFPS;
	float mCurrentTime;
	float mPreviousTime;
	
	void parseJSON(ci::JsonTree json);
	void nextFrame();
	
};