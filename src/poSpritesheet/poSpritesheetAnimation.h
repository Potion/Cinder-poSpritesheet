//
//  poSpritesheetAnimation.h
//  ForestFriendsLayer
//
//  Created by bruce on 10/20/14.
//
//

#pragma once

#include "poSpritesheet.h"

namespace po {
	
	class Spritesheet;
	typedef std::shared_ptr<Spritesheet> SpritesheetRef;
	
	class SpritesheetAnimation;
	typedef std::shared_ptr<SpritesheetAnimation> SpritesheetAnimationRef;
	
	class SpritesheetAnimation
	{
	public:
		static SpritesheetAnimationRef create(SpritesheetRef spritesheet, float fps = 12.f);
		
		~SpritesheetAnimation();
		
		typedef boost::signals2::signal<void(SpritesheetRef)> SignalSpritesheetPlayComplete;
		
		virtual void update();
		virtual void draw();
		void play() { mIsPlaying = true; }
		void pause() { mIsPlaying = false; }
		void stop();
		void setIsLoopingEnabled(bool isLooping) { mIsLooping = isLooping; }
		void setFrameRate(float frameRate);
		
		SignalSpritesheetPlayComplete &getSignalPlayingComplete() { return mPlayCompleteSignal; }
		int getCurrentFrame() { return mCurrentFrame; }
		
	protected:
		SpritesheetAnimation();
		
		void setup(SpritesheetRef spritesheet, float fps);
		
	private:
		SpritesheetRef mSpritesheet;
		
		int mCurrentFrame;
		bool mIsPlaying, mIsLooping;
		float mFrameRate, mFPS, mCurrentTime, mPreviousTime;
		
		SignalSpritesheetPlayComplete mPlayCompleteSignal;
		
		void nextFrame();
		
	};
	
}
