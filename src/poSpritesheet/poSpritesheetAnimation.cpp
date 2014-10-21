//
//  poSpritesheetAnimation.cpp
//  ForestFriendsLayer
//
//  Created by bruce on 10/20/14.
//
//

#include "poSpritesheetAnimation.h"

namespace po {
	
	SpritesheetAnimationRef SpritesheetAnimation::create(SpritesheetRef spritesheet, float fps)
	{
		SpritesheetAnimationRef ref(new SpritesheetAnimation());
		ref->setup(spritesheet, fps);
		return ref;
	}
	
	SpritesheetAnimation::SpritesheetAnimation()
	: mCurrentFrame(0)
	, mIsPlaying(false)
	, mIsLooping(false)
	, mFPS(12.f)
	, mPreviousTime(0.f)
	, mCurrentTime(0.f)
	{}
	
	SpritesheetAnimation::~SpritesheetAnimation()
	{}
	
	void SpritesheetAnimation::setup(SpritesheetRef spritesheet, float fps)
	{
		mSpritesheet = spritesheet;
		setFrameRate(fps);
	}
	
	//
	//	Goto the next frame based on fps
	//
	void SpritesheetAnimation::update()
	{
		mCurrentTime = ci::app::getElapsedSeconds() * 1000;
		float timeDiff = mCurrentTime - mPreviousTime;
		if (timeDiff > mFrameRate) {
			mPreviousTime = mCurrentTime;
			nextFrame();
		}
	}
	
	//
	//	Draw the current frame in the texture
	//
	void SpritesheetAnimation::draw()
	{
		mSpritesheet->drawFrame(mCurrentFrame);
	}
	
	//
	//	Set the frame rate
	//
	void SpritesheetAnimation::setFrameRate(float frameRate)
	{
		mFPS = frameRate;
		mFrameRate = 1000.f / mFPS;
	}
	
	//
	//	Go to the next frame
	//
	void SpritesheetAnimation::nextFrame()
	{
		if (mIsPlaying) {
			mCurrentFrame = (mCurrentFrame += 1) % mSpritesheet->getNumFrames();
			if (!mIsLooping) {
				if (mCurrentFrame == mSpritesheet->getNumFrames() - 1) {
					mIsPlaying = false;
					mPlayCompleteSignal(mSpritesheet);
				}
			}
		}
	}
	
	//
	//	Stop playing
	//
	void SpritesheetAnimation::stop()
	{
		mIsPlaying = false;
		mCurrentFrame = 0;
	}
	
}