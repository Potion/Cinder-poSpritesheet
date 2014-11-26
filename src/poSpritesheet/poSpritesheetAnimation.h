//
//  poSpritesheetAnimation.h
//  ForestFriendsLayer
//
//  Created by bruce on 10/20/14.
//
//

#pragma once

#include "poSpritesheet.h"

#include "boost/signals2.hpp"


namespace po {
	
	class Spritesheet;
	typedef std::shared_ptr<Spritesheet> SpritesheetRef;
	
	class SpritesheetAnimation;
	typedef std::shared_ptr<SpritesheetAnimation> SpritesheetAnimationRef;
	
	class SpritesheetAnimation
	: public std::enable_shared_from_this<SpritesheetAnimation>
	{
	public:
		static SpritesheetAnimationRef create(SpritesheetRef spritesheet, float fps = 12.f);
		
		~SpritesheetAnimation();
		
		typedef boost::signals2::signal<void(SpritesheetAnimationRef)> SignalSpritesheetPlayComplete;
		
		virtual void update();
		virtual void draw();
		void play() { mIsPlaying = true; }
		void pause() { mIsPlaying = false; }
		void stop();
		void setIsLoopingEnabled(bool isLooping) { mIsLooping = isLooping; }
		void setFrameRate(float frameRate);
		void setIsReverse(bool reverse);
		
		SignalSpritesheetPlayComplete &getSignalPlayingComplete() { return mPlayCompleteSignal; }
		int getCurrentFrame() { return mCurrentFrame; }
		
	protected:
		SpritesheetAnimation();
		
		void setup(SpritesheetRef spritesheet, float fps);
		
	private:
		SpritesheetRef mSpritesheet;
		
		int mCurrentFrame;
		bool mIsPlaying, mIsLooping, mIsReverse;
		float mFrameRate, mFPS, mCurrentTime, mPreviousTime;
		int mLastFrame;
		
		SignalSpritesheetPlayComplete mPlayCompleteSignal;
		
		void nextFrame();
		
	};
	
}
