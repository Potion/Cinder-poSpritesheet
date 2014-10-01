//
//  Spritesheet.cpp
//  
//
//  Created by bruce on 8/26/14.
//
//

#include "poSpritesheet.h"

namespace po {
	
	SpritesheetRef Spritesheet::create(ci::gl::TextureRef texture, ci::JsonTree json)
	{
		SpritesheetRef ref(new Spritesheet());
		ref->setup(texture, json);
		return ref;
	}
	
	SpritesheetRef Spritesheet::create(std::vector<ci::gl::TextureRef> &textures, std::vector<ci::JsonTree> &data)
	{
		SpritesheetRef ref(new Spritesheet());
		ref->setupMultipack(textures, data);
		return ref;
	}
	
	Spritesheet::Spritesheet()
	: mCurrentFrame(0)
	, mIsPlaying(false)
	, mIsLooping(false)
	, mFPS(12.f)
	, mPreviousTime(0.f)
	, mCurrentTime(0.f)
	, mIsDrawOriginalBounds(false)
	, mIsDrawFrameBounds(false)
	, mCurrentFrameKey("")
	{}
	
	Spritesheet::~Spritesheet()
	{}
	
	//
	//	Single texture spritesheet setup
	//
	void Spritesheet::setup(ci::gl::TextureRef texture, ci::JsonTree json)
	{
		setFrameRate(mFPS);
//		setupSpriteMap(texture, json);
		
		mTextures[0] = texture;
		setupSpriteMap(0, json);
	}
	
	//
	//	Multipacked spritesheet setup
	//
	void Spritesheet::setupMultipack(std::vector<ci::gl::TextureRef> &textures, std::vector<ci::JsonTree> data)
	{
		setFrameRate(mFPS);
		
		int counter = 0;
		for (auto json : data) {
			int textureID = counter;
			mTextures[textureID] = textures[counter];
//			ci::gl::TextureRef texture = textures[counter];
//			setupSpriteMap(texture, json);
			setupSpriteMap(textureID, json);
			counter++;
		}
	}
	
	//
	//	Setup frame data and texture maps
	//
//	void Spritesheet::setupSpriteMap(ci::gl::TextureRef texture, ci::JsonTree json)
	void Spritesheet::setupSpriteMap(int textureID, ci::JsonTree json)
	{
		// get all the frames in the json
		for (auto frame : json.getChild("frames")) {
			FrameData frameData = getFrameData(frame);
			std::string frameKey = frameData.filename;
			mFrameData[frameKey] = frameData;
//			mTextures[frameKey] = texture;
			mTextureIDs[frameKey] = textureID;
			mFrameOrder.push_back(frameKey);
		}
		
		// sort the frame order alphabetically
		std::sort(mFrameOrder.begin(), mFrameOrder.end());
		mNumFrames = mFrameOrder.size();
	}
	
	//
	//	Update method should be called in the parent update method
	//
	void Spritesheet::update()
	{
		mCurrentTime = ci::app::getElapsedSeconds() * 1000;
		float timeDiff = mCurrentTime - mPreviousTime;
		if (timeDiff > mFrameRate) {
			mPreviousTime = mCurrentTime;
			nextFrame();
		}
	}
	
	//
	//	Draw method should be called from the parent draw method
	//
	void Spritesheet::draw()
	{
		ci::gl::pushMatrices();
		drawBounds();
		drawFrame();
		ci::gl::popMatrices();
	}
	
	//
	//	Create framedata object from json
	//
	Spritesheet::FrameData Spritesheet::getFrameData(ci::JsonTree json)
	{
		FrameData frameData = FrameData();
		frameData.filename = json.getChild("filename").getValue<std::string>();
		frameData.frame = ci::Area(
			json.getChild("frame").getChild("x").getValue<float>(),
			json.getChild("frame").getChild("y").getValue<float>(),
			json.getChild("frame").getChild("x").getValue<float>() +
			json.getChild("frame").getChild("w").getValue<float>(),
			json.getChild("frame").getChild("y").getValue<float>() +
			json.getChild("frame").getChild("h").getValue<float>()
		);
		frameData.rotated = json.getChild("rotated").getValue<bool>();
		frameData.trimmed = json.getChild("trimmed").getValue<bool>();
		frameData.spriteSourceSize = ci::Rectf(
			json.getChild("spriteSourceSize").getChild("x").getValue<float>(),
			json.getChild("spriteSourceSize").getChild("y").getValue<float>(),
			json.getChild("spriteSourceSize").getChild("x").getValue<float>() +
			json.getChild("spriteSourceSize").getChild("w").getValue<float>(),
			json.getChild("spriteSourceSize").getChild("y").getValue<float>() +
			json.getChild("spriteSourceSize").getChild("h").getValue<float>()
		);
		frameData.sourceSize = ci::Vec2f(
			json.getChild("sourceSize").getChild("w").getValue<float>(),
			json.getChild("sourceSize").getChild("h").getValue<float>()
		);
		
		return frameData;
	}
	
	//
	//	Proceed to the next frame
	//	If it's not looping send a complete signal
	//
	void Spritesheet::nextFrame()
	{
		if (mIsPlaying) {
			mCurrentFrame = (mCurrentFrame += 1) % mNumFrames;
			mCurrentFrameKey = mFrameOrder[mCurrentFrame];
//			ci::app::console() << "Spritesheet::drawFrame: " << mCurrentFrameKey << std::endl;
			if (!mIsLooping) {
				if (mCurrentFrame == mNumFrames - 1) {
					mIsPlaying = false;
					mPlayCompleteSignal(shared_from_this());
				}
			}
		}
	}
	
	//
	//	Draw bounds for the original size or the current frame size
	//
	void Spritesheet::drawBounds()
	{
		ci::gl::color(ci::Color(1,0,0));
		
		if (mIsDrawOriginalBounds) {
			ci::gl::drawStrokedRect(getOriginalBounds());
		}
		
		if (mIsDrawFrameBounds) {
			ci::gl::drawStrokedRect(getFrameBounds());
		}
		
		ci::gl::color(ci::Color(1,1,1));
	}
	
	//
	//	Draw the texture for the current frame
	//
	void Spritesheet::drawFrame()
	{
		ci::gl::enableAlphaBlending();
		
//		ci::gl::draw(mTextures[mCurrentFrameKey], mFrameData[mCurrentFrameKey].frame, mFrameData[mCurrentFrameKey].spriteSourceSize);
		
		ci::gl::draw(mTextures[mTextureIDs[mCurrentFrameKey]], mFrameData[mCurrentFrameKey].frame, mFrameData[mCurrentFrameKey].spriteSourceSize);
		
		ci::gl::disableAlphaBlending();
	}
	
	//
	//	Get the original bounds for the frame
	//
	ci::Rectf Spritesheet::getOriginalBounds()
	{
		ci::Rectf bounds(0, 0, mFrameData[mCurrentFrameKey].sourceSize.x, mFrameData[mCurrentFrameKey].sourceSize.y);
		return bounds;
	}
	
	//
	//	Get the frame bounds for the current frame
	//
	ci::Rectf Spritesheet::getFrameBounds()
	{
		return mFrameData[mCurrentFrameKey].spriteSourceSize;
	}
	
	//
	//	Set frames/second
	//
	void Spritesheet::setFrameRate(float frameRate)
	{
		mFPS = frameRate;
		mFrameRate = 1000.f / mFPS;
	}
	
	//
	//	Stop playback
	//
	void Spritesheet::stop()
	{
		mIsPlaying = false;
		mCurrentFrame = 0;
	}
	
}